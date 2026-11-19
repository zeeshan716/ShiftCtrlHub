import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:http/http.dart' as http;
import 'package:path_provider/path_provider.dart';
import 'package:webview_flutter/webview_flutter.dart';

const String kRemoteUiUrl = 'https://zeeshan716.github.io/ShiftCtrlHub/index.html';
const String kBundledAssetPath = 'assets/web/index.html';
const String kCachedFilename = 'cached_index.html';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await SystemChrome.setPreferredOrientations([
    DeviceOrientation.landscapeLeft,
    DeviceOrientation.landscapeRight,
  ]);
  await SystemChrome.setEnabledSystemUIMode(SystemUiMode.immersiveSticky);
  runApp(const ShiftCtrlHubShellApp());
}

class ShiftCtrlHubShellApp extends StatelessWidget {
  const ShiftCtrlHubShellApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'ShiftCtrlHub',
      debugShowCheckedModeBanner: false,
      theme: ThemeData.dark().copyWith(
        scaffoldBackgroundColor: const Color(0xFF050507),
      ),
      home: const LiveWebShellPage(),
    );
  }
}

class LiveWebShellPage extends StatefulWidget {
  const LiveWebShellPage({super.key});

  @override
  State<LiveWebShellPage> createState() => _LiveWebShellPageState();
}

class _LiveWebShellPageState extends State<LiveWebShellPage> {
  late final WebViewController _webController;
  RawDatagramSocket? _udpSocket;
  File? _cachedFile;
  bool _webReady = false;
  bool _updateBannerVisible = false;
  String? _statusNotice;

  @override
  void initState() {
    super.initState();
    _initController();
    _initUdpSocket();
    _bootOfflineFirstThenSync();
  }

  void _initController() {
    _webController = WebViewController()
      ..setJavaScriptMode(JavaScriptMode.unrestricted)
      ..setBackgroundColor(const Color(0xFF050507))
      ..addJavaScriptChannel(
        'ShiftCtrlNative',
        onMessageReceived: (JavaScriptMessage message) {
          _handleBridgeMessage(message.message);
        },
      )
      ..setNavigationDelegate(
        NavigationDelegate(
          onPageFinished: (_) {
            if (mounted) {
              _webController.runJavaScript('applyNativeShellLayout && applyNativeShellLayout();');
            }
          },
        ),
      );
  }

  Future<void> _initUdpSocket() async {
    try {
      final sock = await RawDatagramSocket.bind(InternetAddress.anyIPv4, 0);
      sock.broadcastEnabled = true;
      sock.listen((RawSocketEvent event) {
        if (event != RawSocketEvent.read) return;
        final datagram = sock.receive();
        if (datagram == null) return;
        final text = utf8.decode(datagram.data, allowMalformed: true).trim();
        if (text.startsWith('IP:')) {
          final discovered = text.substring(3).trim();
          final payload = jsonEncode({'discoveredIp': discovered});
          if (mounted && _webReady) {
            _webController.runJavaScript(
              'window.onShiftCtrlNativeEvent && window.onShiftCtrlNativeEvent($payload);',
            );
          }
        }
      });
      _udpSocket = sock;
    } catch (_) {
      // Deferred retry when user transmits
    }
  }

  /// Boot flow:
  /// 1. Ensure cached_index.html exists (seeded from bundled APK asset on Day 1).
  /// 2. Load cached_index.html immediately (0ms offline launch).
  /// 3. In background, check GitHub Pages for a newer index.html; if updated,
  ///    overwrite cached_index.html so offline & future launches stay current.
  Future<void> _bootOfflineFirstThenSync() async {
    final docsDir = await getApplicationDocumentsDirectory();
    final cacheFile = File('${docsDir.path}/$kCachedFilename');
    _cachedFile = cacheFile;

    String currentHtml;
    if (await cacheFile.exists()) {
      currentHtml = await cacheFile.readAsString();
    } else {
      currentHtml = await rootBundle.loadString(kBundledAssetPath);
      await cacheFile.writeAsString(currentHtml);
    }

    await _webController.loadFile(cacheFile.path);
    if (mounted) {
      setState(() => _webReady = true);
    }

    // Background cloud UI refresh (non-blocking; ignores failures when offline)
    unawaited(_syncRemoteUiInBackground(cacheFile, currentHtml));
  }

  Future<void> _syncRemoteUiInBackground(File cacheFile, String currentHtml) async {
    try {
      final response = await http
          .get(Uri.parse(kRemoteUiUrl))
          .timeout(const Duration(seconds: 4));

      if (response.statusCode == 200) {
        final remoteHtml = response.body;
        // Sanity check so router captive-portal pages never corrupt the cache
        final isValidUi = remoteHtml.contains('ShiftCtrlHub') &&
            remoteHtml.contains('ShiftCtrlNative');
        if (isValidUi && remoteHtml != currentHtml) {
          await cacheFile.writeAsString(remoteHtml);
          if (mounted) {
            setState(() => _updateBannerVisible = true);
          }
        }
      }
    } catch (_) {
      // Offline or arena Wi-Fi without internet: keep using cached_index.html silently
    }
  }

  Future<void> _reloadFromCache() async {
    final file = _cachedFile;
    if (file == null) return;
    await _webController.loadFile(file.path);
    if (mounted) {
      setState(() => _updateBannerVisible = false);
    }
  }

  void _handleBridgeMessage(String rawJson) {
    try {
      final decoded = jsonDecode(rawJson) as Map<String, dynamic>;
      final transport = (decoded['transport'] ?? 'softap').toString().toLowerCase();
      final token = (decoded['token'] ?? '').toString();
      final ip = (decoded['ip'] ?? '192.168.4.1').toString();
      final portRaw = decoded['port'];
      final port = portRaw is int ? portRaw : (int.tryParse('$portRaw') ?? 3333);

      if (token.isEmpty) return;

      if (transport == 'softap' || transport == 'sta') {
        _sendUdpToken(token, ip, port);
        if (_statusNotice != null && mounted) {
          setState(() => _statusNotice = null);
        }
      } else if (transport == 'ble' || transport == 'spp') {
        if (mounted) {
          setState(() {
            _statusNotice =
                '${transport.toUpperCase()} radio driver coming in next mobile update — use SoftAP / STA Wi-Fi today.';
          });
        }
      }
    } catch (_) {
      // Ignore malformed bridge payloads
    }
  }

  void _sendUdpToken(String token, String ip, int port) {
    final sock = _udpSocket;
    if (sock == null) {
      unawaited(_initUdpSocket());
      return;
    }
    try {
      final addr = InternetAddress(ip);
      sock.send(utf8.encode('$token\n'), addr, port);
    } catch (_) {
      // Ignore transient network errors when Wi-Fi is switching
    }
  }

  @override
  void dispose() {
    _udpSocket?.close();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF050507),
      body: SafeArea(
        child: Stack(
          children: [
            if (_webReady)
              WebViewWidget(controller: _webController)
            else
              const Center(
                child: CircularProgressIndicator(color: Color(0xFF6D001A)),
              ),
            if (_updateBannerVisible)
              Positioned(
                top: 8,
                left: 0,
                right: 0,
                child: Center(
                  child: Material(
                    color: const Color(0xFF6D001A),
                    borderRadius: BorderRadius.circular(20),
                    elevation: 6,
                    child: InkWell(
                      borderRadius: BorderRadius.circular(20),
                      onTap: _reloadFromCache,
                      child: const Padding(
                        padding: EdgeInsets.symmetric(horizontal: 14, vertical: 6),
                        child: Text(
                          'New UI cached from GitHub — Tap to apply',
                          style: TextStyle(
                            color: Colors.white,
                            fontSize: 11,
                            fontWeight: FontWeight.w700,
                          ),
                        ),
                      ),
                    ),
                  ),
                ),
              ),
            if (_statusNotice != null)
              Positioned(
                bottom: 36,
                left: 16,
                right: 16,
                child: Center(
                  child: Container(
                    padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 6),
                    decoration: BoxDecoration(
                      color: const Color(0xE6141418),
                      borderRadius: BorderRadius.circular(8),
                      border: Border.all(color: const Color(0xFF6D001A)),
                    ),
                    child: Text(
                      _statusNotice!,
                      style: const TextStyle(color: Colors.white, fontSize: 11),
                    ),
                  ),
                ),
              ),
          ],
        ),
      ),
    );
  }
}
