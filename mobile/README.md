# ShiftCtrlHub Mobile App — Live Web Shell (Beta)

> **Architecture (Option 3 — "Install Once, Live UI Updates"):**
> Built to eliminate APK reinstalls whenever the remote UI changes.

## How It Works
1. **Bundled Offline Fallback (`assets/web/index.html`):** Ships inside the APK so the app launches with zero delay even on Day 1 without internet.
2. **Local Persistent Cache (`cached_index.html`):** Whenever the phone has internet, the app checks `https://zeeshan716.github.io/ShiftCtrlHub/index.html` in the background and saves any new UI update locally.
3. **100% Offline Arena Driving:** If the user has no internet (connected only to the robot's `ShiftCtrlHub` hotspot), the shell loads the **most recently synced `cached_index.html`** immediately (`0 ms` lag).
4. **Native Radio Bridge (`ShiftCtrlNative`):** Every button/slider/joystick event in the WebView calls native Dart memory (`< 1 ms`) to transmit raw Wi-Fi UDP (`port 3333`) datagrams directly to your robot — **zero internet lag, zero Python PC bridge needed**.

## Transport Support in Mobile Shell
- [x] **Wi-Fi SoftAP (`192.168.4.1:3333`)** via native `RawDatagramSocket`
- [x] **Wi-Fi STA (Router IP / UDP Discovery)** via native `RawDatagramSocket`
- [ ] **Bluetooth Low Energy (`BLE`)** native radio plugin
- [ ] **Bluetooth Classic (`SPP`)** native radio plugin

## Building the APK
### Automated (GitHub Actions)
Push to `main` (or trigger **Build Android Beta APK** in the repo's **Actions** tab) to build and download `ShiftCtrlHub-Android-Beta.apk` directly from GitHub.

### Local (Requires Flutter SDK)
```bash
cp ../index.html assets/web/index.html
cd mobile
flutter create --platforms=android --org com.zeeshan716 --project-name shiftctrlhub_mobile .
flutter build apk --release
```
