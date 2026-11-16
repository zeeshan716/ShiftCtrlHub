import 'dart:convert';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await SystemChrome.setPreferredOrientations([
    DeviceOrientation.landscapeLeft,
    DeviceOrientation.landscapeRight,
  ]);
  runApp(const ShiftCtrlHubApp());
}

class ShiftCtrlHubApp extends StatelessWidget {
  const ShiftCtrlHubApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'ShiftCtrlHub',
      debugShowCheckedModeBanner: false,
      theme: ThemeData.dark().copyWith(
        scaffoldBackgroundColor: const Color(0xFF050507),
        colorScheme: const ColorScheme.dark(
          primary: Color(0xFF6D001A),
          surface: Color(0xFF0C0C0E),
        ),
      ),
      home: const CockpitPage(),
    );
  }
}

class CockpitPage extends StatefulWidget {
  const CockpitPage({super.key});

  @override
  State<CockpitPage> createState() => _CockpitPageState();
}

class _CockpitPageState extends State<CockpitPage> {
  String _targetIp = '192.168.4.1';
  int _targetPort = 3333;
  double _speed = 180;
  String _lastTx = 'READY';
  RawDatagramSocket? _udpSocket;

  @override
  void initState() {
    super.initState();
    _initUdp();
  }

  Future<void> _initUdp() async {
    try {
      _udpSocket = await RawDatagramSocket.bind(InternetAddress.anyIPv4, 0);
    } catch (_) {
      // Socket init deferred until retry or OS network grant
    }
  }

  void _sendToken(String token) {
    setState(() => _lastTx = token);
    final socket = _udpSocket;
    if (socket == null) return;
    try {
      final addr = InternetAddress(_targetIp);
      socket.send(utf8.encode('$token\n'), addr, _targetPort);
    } catch (_) {
      // Ignore transient network send errors during standby
    }
  }

  @override
  void dispose() {
    _udpSocket?.close();
    super.dispose();
  }

  Widget _dirKey(IconData icon, String cmd) {
    return GestureDetector(
      onTapDown: (_) => _sendToken(cmd),
      onTapUp: (_) => _sendToken('C:S'),
      onTapCancel: () => _sendToken('C:S'),
      child: Container(
        width: 68,
        height: 68,
        decoration: BoxDecoration(
          color: const Color(0xFF1B1B22),
          borderRadius: BorderRadius.circular(14),
          border: Border.all(color: const Color(0xFF272730)),
        ),
        child: Icon(icon, color: Colors.white, size: 30),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 12),
          child: Column(
            children: [
              // Top header bar
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  const Row(
                    children: [
                      Icon(Icons.circle, color: Color(0xFF10B981), size: 10),
                      SizedBox(width: 8),
                      Text(
                        'ShiftCtrlHub',
                        style: TextStyle(
                          fontSize: 16,
                          fontWeight: FontWeight.w700,
                          letterSpacing: 0.6,
                        ),
                      ),
                    ],
                  ),
                  Text(
                    '$_targetIp:$_targetPort  |  UDP',
                    style: const TextStyle(color: Color(0xFF8F8F9D), fontSize: 12),
                  ),
                ],
              ),
              const Divider(color: Color(0xFF272730), height: 20),

              // Main dual-thumb cockpit
              Expanded(
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    // Left thumb: vertical speed throttle
                    Column(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        Container(
                          padding: const EdgeInsets.symmetric(horizontal: 10, vertical: 4),
                          decoration: BoxDecoration(
                            color: const Color(0xFF141418),
                            borderRadius: BorderRadius.circular(6),
                            border: Border.all(color: const Color(0xFFD97706)),
                          ),
                          child: Text(
                            _speed.round().toString(),
                            style: const TextStyle(fontFamily: 'monospace', fontWeight: FontWeight.bold),
                          ),
                        ),
                        Expanded(
                          child: RotatedBox(
                            quarterTurns: 3,
                            child: Slider(
                              value: _speed,
                              min: 0,
                              max: 255,
                              activeColor: const Color(0xFF6D001A),
                              onChanged: (v) {
                                setState(() => _speed = v);
                                _sendToken('S:${v.round()}');
                              },
                            ),
                          ),
                        ),
                      ],
                    ),

                    // Right thumb: 4WD D-Pad diamond
                    Column(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        _dirKey(Icons.arrow_upward, 'C:F'),
                        const SizedBox(height: 10),
                        Row(
                          children: [
                            _dirKey(Icons.arrow_back, 'C:L'),
                            const SizedBox(width: 78),
                            _dirKey(Icons.arrow_forward, 'C:R'),
                          ],
                        ),
                        const SizedBox(height: 10),
                        _dirKey(Icons.arrow_downward, 'C:B'),
                      ],
                    ),
                  ],
                ),
              ),

              // Footer rail
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Text('TX: $_lastTx', style: const TextStyle(fontFamily: 'monospace', fontSize: 12)),
                  const Text('RC // 4WD BETA FLUTTER CLIENT',
                      style: TextStyle(color: Color(0xFF8F8F9D), fontSize: 11)),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}
