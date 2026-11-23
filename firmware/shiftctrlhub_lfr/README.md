# ShiftCtrlHub — LFR-Only PIDV Firmware

Dedicated autonomous **Line Follower Robot (LFR)** firmware with real-time wireless PID + velocity-damping (`Kv`) tuning via **ShiftCtrlHub**.

## Files
* `shiftctrlhub_lfr.ino` — Main sketch & PIDV tuning command handler (`MODE:LFR`, `C:F`/`C:S`, `V:*`)
* `config.h` — Transport selector (`ENABLE_HYBRID_WIFI` / `ACTIVE_TRANSPORT`), motor pins, and 5-IR ADC1 sensor pins
* `lfr_pidv.h` — Weighted 5-sensor PIDV loop + line-loss recovery
* `robot_drive.h` — Differential drive motor PWM (`analogWrite`)
* `transports.h` — Hybrid Auto-Wi-Fi (`STA -> SoftAP`), SoftAP, STA, BLE, and SPP transport layer
