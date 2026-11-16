# ShiftCtrlHub Mobile App (Beta Skeleton)

> **Status:** `v0.1.0-beta` starter scaffold — kept in parallel with the web interface (`index.html`) and modular MCU firmware (`firmware/shiftctrlhub/`).

## What Is Included Right Now
* `pubspec.yaml` — Flutter project manifest (pure Flutter + Dart standard library networking, zero fragile native plugin dependencies yet).
* `lib/main.dart` — Landscape cockpit starter matching ShiftCtrlHub's Onyx Dark & Burgundy theme with working raw UDP socket transmission (`RawDatagramSocket` → `192.168.4.1:3333`) for `C:F`, `C:B`, `C:L`, `C:R`, `C:S`, and `S:<speed>` throttle slider tokens.

## Feature Parity Roadmap (vs. Web UI)
- [x] Landscape 4WD Cardinal D-Pad + Master Speed Slider (`C:*`, `S:*`) over Wi-Fi UDP
- [ ] 2WD Dual Auto-Centering Sliders (`S:FB:*`, `S:RL:*`)
- [ ] 360° Analog Virtual Joystick (`JOY:<X>:<Y>`)
- [ ] LFR Autonomous PIDV Tuning Console (`V:ALL:*`)
- [ ] Bluetooth Low Energy (BLE) & Classic Bluetooth (SPP) transport drivers
- [ ] Full Android / iOS native wrapper (`flutter create .`) + CI APK build

## Running Locally (Requires Flutter SDK)
```bash
cd mobile
flutter create .
flutter run
```
