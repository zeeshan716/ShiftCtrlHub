# ShiftCtrlHub — Universal Remote Control & Robotics Framework

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Target: Multi-MCU](https://img.shields.io/badge/Target-Multi--MCU-blue.svg)]()
[![Protocol: UDP_3333](https://img.shields.io/badge/Protocol-UDP%203333-brightgreen.svg)]()
[![Theme: Burgundy_Titanium](https://img.shields.io/badge/Theme-Burgundy%20Titanium-800020.svg)]()
[![Documentation: Verified](https://img.shields.io/badge/Documentation-Catalog%20PDF-purple.svg)]()

> **ShiftCtrlHub** is a zero-cost, open-source universal robotics remote control interface and firmware framework designed for microcontrollers (ESP32, Arduino, Raspberry Pi, STM32). It features an ultra-responsive, ergonomic mobile touch interface paired with modular firmware supporting dual drive architectures and real-time autonomous PIDV tuning.

---

## 🚀 Key Features

* **Dual Drive Architectures**:
  * **4WD Differential-Drive / Skid-Steer**: 4 cardinal motion commands (`C:F`, `C:B`, `C:L`, `C:R`, `C:S`) with continuous master PWM throttle (`0`–`255`).
  * **2WD Front-Wheel Steer (Toy-Car / Real-Car Mechanics)**: Auto-centering vertical drive motor throttle (`-100%` to `+100%`) paired with horizontal front-wheel steering servo angle (`-100%` Left to `+100%` Right).
* **360° Analog Virtual Joystick**: Draggable circular thumbstick with dynamic polar and Cartesian deflection mixing, HUD telemetry readouts, and configurable neutral deadband.
* **Line Follower Robot (LFR) PIDV Console**: Live parameter streaming for autonomous line tracking:
  * Base Speed Cruise PWM (`0`–`255`)
  * Proportional Gain ($K_p$, `0.0`–`10.0`)
  * Integral Gain ($K_i$, `0.00`–`2.00`)
  * Derivative Gain ($K_d$, `0.0`–`10.0`)
  * Velocity Scaling Corner Damping ($K_v$, `0.0`–`5.0`)
  * One-tap benchmark tuning presets (Default, High Speed, Hairpin)
* **4 Unified Wireless Transport Mediums**:
  1. **Wi-Fi SoftAP**: Direct hotspot connection (`192.168.4.1`, UDP port `3333`).
  2. **Wi-Fi Station Mode (STA)**: Router network connection with `WiFiMulti` multi-AP auto-reconnect and UDP broadcast auto-discovery.
  3. **Bluetooth Low Energy (BLE)**: GATT UART service compatible with modern mobile platforms.
  4. **Classic Bluetooth SPP**: Direct serial pairing with HC-05 modules and Bluetooth serial microcontrollers.

---

## 📁 Repository Structure

```
.
├── index.html                           # Production remote control web interface (GitHub Pages entry)
├── ShiftCtrlHub_Components_Catalog.pdf  # Official 3-column component catalog with reference snapshots
├── remote_parts_catalog.md              # Standardized Markdown catalog with part identifiers
├── firmware/
│   └── shiftctrlhub/                    # Modular multi-MCU robotics firmware
│       ├── shiftctrlhub.ino             # Main sketch setup() and loop() + command processor
│       ├── config.h                     # Hardware pinouts, multi-AP Wi-Fi credentials, and settings
│       ├── robot_drive.h                # 4WD skid-steer & 2WD servo steer kinematics
│       ├── lfr_pidv.h                   # 5-IR sensor weighted PIDV line follower & pattern recovery
│       ├── transports.h                 # Wireless transport engines (STA UDP, SoftAP, BLE, SPP)
│       └── README.md                    # Detailed firmware flashing and wiring guide
├── mobile/
│   ├── pubspec.yaml                     # Flutter mobile client manifest (Beta skeleton)
│   ├── README.md                        # Mobile app parity & build notes
│   └── lib/main.dart                    # Landscape cockpit + raw UDP driver
├── tools/
│   ├── shiftctrlhub_server.py           # Local web & UDP datagram relay bridge
│   └── udp_bridge.py                    # Standalone lightweight UDP forwarder
└── LICENSE                              # MIT open-source license
```

---

## 🛠️ Default Reference Pinout (Example Wiring)

| Function | Reference Pin | Target Component | Notes |
| :--- | :--- | :--- | :--- |
| **Motor IN1** | `GPIO 18` | Left Motor Forward (PWM) | L298N / TB6612 / DRV8833 |
| **Motor IN2** | `GPIO 19` | Left Motor Reverse (PWM) | H-Bridge Motor Driver |
| **Motor IN3** | `GPIO 22` | Right Motor Forward (PWM) | H-Bridge Motor Driver |
| **Motor IN4** | `GPIO 23` | Right Motor Reverse (PWM) | H-Bridge Motor Driver |
| **Steering Servo** | `GPIO 21` | Front-Wheel Steering Servo | SG90 / MG90S (50 Hz PWM) |
| **LFR Sensor S1** | `GPIO 34` | Far Left IR Sensor | TCRT5000 / QTR-5A Array |
| **LFR Sensor S2** | `GPIO 35` | Mid Left IR Sensor | Analog / Comparator Input |
| **LFR Sensor S3** | `GPIO 32` | Center IR Sensor | Tracking Line Reference |
| **LFR Sensor S4** | `GPIO 33` | Mid Right IR Sensor | Analog / Comparator Input |
| **LFR Sensor S5** | `GPIO 25` | Far Right IR Sensor | TCRT5000 / QTR-5A Array |

*(All pins are fully customizable for your microcontroller in `firmware/shiftctrlhub/config.h`)*.

---

## 🌐 Quick Start (Running the Remote)

### Local Testing (PC & Mobile Phone on Wi-Fi)
1. Run the local unified server:
   ```bash
   python tools/shiftctrlhub_server.py
   ```
2. Open the interface:
   * **On your Computer**: `http://localhost:8080/`
   * **On your Mobile Phone**: `http://<your-lan-ip>:8080/`
3. Touch interactions immediately forward as raw UDP datagrams to your robot microcontroller on port `3333`!

---

## 📄 Component & Parts Catalog

To maintain zero collateral UI drift, every visual element in ShiftCtrlHub has a standardized **Part Code** and cropped photographic reference. Refer to:
* 📕 [ShiftCtrlHub_Components_Catalog.pdf](ShiftCtrlHub_Components_Catalog.pdf)
* 📋 [remote_parts_catalog.md](remote_parts_catalog.md)

---

## 📜 License

This project is open-source and released under the [MIT License](LICENSE).
