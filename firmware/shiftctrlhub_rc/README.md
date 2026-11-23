# ShiftCtrlHub — RC-Only Firmware

Dedicated manual radio-control firmware for **4WD Differential-Drive** and **2WD Front-Wheel Steer** robots without LFR sensor overhead.

## Files
* `shiftctrlhub_rc.ino` — Main sketch & RC command dispatcher (`C:*`, `S:*`, `S:FB:*`, `S:RL:*`, `DRIVE:*`, `JOY:*`)
* `config.h` — Transport selector (`ENABLE_HYBRID_WIFI` / `ACTIVE_TRANSPORT`), Wi-Fi/BT settings, and motor/servo pins
* `robot_drive.h` — `analogWrite()` H-bridge PWM and 50 Hz servo control
* `transports.h` — Hybrid Auto-Wi-Fi (`STA -> SoftAP`), SoftAP, STA, BLE, and SPP transport layer
