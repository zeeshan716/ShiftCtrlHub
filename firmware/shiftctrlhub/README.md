# ShiftCtrlHub — Universal Robotics Firmware Guide

This firmware turns compatible microcontrollers into a universal robotics controller communicating in real time with the **ShiftCtrlHub** interface *(default reference pinout and Arduino Wi-Fi/BLE transport drivers target ESP32-class boards, with multi-MCU support expanding)*.

---

## 📁 Firmware Structure

```
firmware/shiftctrlhub/
├── shiftctrlhub.ino           # Main sketch setup() and loop() + command processor
├── config.h                 # Hardware pinouts, multi-AP credentials, & active transport mode
├── robot_drive.h            # 4WD Differential-Drive (skid-steer) & 2WD Front-Wheel Steer (servo)
├── lfr_pidv.h               # 5-IR sensor weighted PIDV line follower & loss recovery algorithm
└── transports.h             # 4 wireless transport engines (STA WiFiMulti, SoftAP, BLE, SPP)
```

---

## ⚡ 1. How to Select the Active Connection Mode

In `config.h`, you can enable **Hybrid Auto-Wi-Fi** or lock strictly to a single transport mode:

```cpp
// Hybrid Wi-Fi ON/OFF toggle:
//   1 = Try STA_NETWORKS for 10s first (takes priority), fallback to SoftAP if none connect
//   0 = OFF (strictly use ONLY the single ACTIVE_TRANSPORT chosen below)
#define ENABLE_HYBRID_WIFI    1

// Choose strict single mode (or BLE / SPP):
#define ACTIVE_TRANSPORT CONN_SOFTAP  // Direct Hotspot hosted by the robot MCU (UDP 3333)
// #define ACTIVE_TRANSPORT CONN_STA     // Wi-Fi Station with multi-AP auto-failover (UDP 3333)
// #define ACTIVE_TRANSPORT CONN_BLE     // Bluetooth Low Energy GATT Server
// #define ACTIVE_TRANSPORT CONN_SPP     // Classic Bluetooth Serial (Pair with "ShiftCtrlHub")
```

---

## 🌐 2. Adding Multiple Wi-Fi Credentials (STA Mode)

In `config.h`, you can add as many networks as you need inside `STA_NETWORKS`. `WiFiMulti` will automatically connect to the strongest available network and cycle between them if signal drops:

```cpp
const WiFiCredential STA_NETWORKS[] = {
  { "YourWiFiSSID",                "YourWiFiPassword" },
  { "MyHomeWiFi",                  "HomePassword123" },
  { "PhoneHotspot",                "HotspotPass123" }
};
```

### Automatic UDP Discovery in STA Mode:
When the robot joins your local network, the remote app can broadcast `"Discovery_Request"` to `255.255.255.255:3333`. The microcontroller immediately replies with `"IP:<robot-ip>"`, auto-filling the remote's target socket!

---

## 🚗 3. Default Reference Pinout Map

| Feature | Reference Pin | Target Component |
| :--- | :--- | :--- |
| **Motor IN1** | `GPIO 18` (`analogWrite` PWM) | Left Motor Forward (L298N, keep ENA/ENB jumpers on) |
| **Motor IN2** | `GPIO 19` (`analogWrite` PWM) | Left Motor Reverse |
| **Motor IN3** | `GPIO 22` (`analogWrite` PWM) | Right Motor Forward |
| **Motor IN4** | `GPIO 23` (`analogWrite` PWM) | Right Motor Reverse |
| **Steering Servo** | `GPIO 21` (50Hz PWM) | 2WD Front-Wheel Steering Servo (SG90/MG90) |
| **LFR Sensor S1** | `GPIO 34` (Analog IN) | Far Left IR Reflective Sensor |
| **LFR Sensor S2** | `GPIO 35` (Analog IN) | Mid Left IR Reflective Sensor |
| **LFR Sensor S3** | `GPIO 32` (Analog IN) | Center IR Reflective Sensor |
| **LFR Sensor S4** | `GPIO 33` (Analog IN) | Mid Right IR Reflective Sensor |
| **LFR Sensor S5** | `GPIO 36` (Analog IN) | Far Right IR Reflective Sensor |

*(All pins can be customized in `config.h`)*.

---

## 📡 4. Command Protocol Reference

The microcontroller processes the following real-time tokens:

| Token | Payload | Action |
| :--- | :--- | :--- |
| `C:F` | Forward | Moves forward at current master speed. |
| `C:B` | Backward | Moves reverse at current master speed. |
| `C:L` | Left | Spins left (4WD) or turns left (2WD). |
| `C:R` | Right | Spins right (4WD) or turns right (2WD). |
| `C:S` | Stop | Immediately stops all drive motors. |
| `S:<0-255>` | Master Throttle | Sets base PWM speed limit (e.g. `S:180`). |
| `S:FB:<-100 to 100>` | 2WD Drive Throttle | Forward/reverse drive percentage in 2WD mode. |
| `S:RL:<-100 to 100>` | 2WD Wheel Angle | Front steering servo deflection percentage (-100% Left, 0% Center, +100% Right). |
| `JOY:<X>:<Y>` | Analog Joystick | Deflection vector for omnidirectional skid mixing. |
| `V:Speed:<val>` | LFR Cruise Speed | Updates autonomous base cruise PWM. |
| `V:Kp:<val>` | Proportional Gain | Sets line tracking gain in real time. |
| `V:Ki:<val>` | Integral Gain | Sets steady-state error compensation. |
| `V:Kd:<val>` | Derivative Gain | Sets damping / oscillation suppression. |
| `V:Kv:<val>` | Velocity Scaling | Sets cornering speed deceleration factor. |
| `MODE:RC` / `MODE:LFR` | Mode Switch | Toggles between manual control and autonomous PIDV. |
| `DRIVE:4V4` / `DRIVE:2WD`| Architecture Switch| Switches between 4WD Skid-Steer and 2WD Car. |

---

## 💻 5. Local Browser Testing with `udp_bridge.py`

Browsers sandbox raw UDP sockets, so to test the HTML remote (`index.html`) on your desktop before packaging to mobile:

1. Run the local bridge in PowerShell or terminal:
   ```powershell
   python tools/udp_bridge.py
   ```
2. Open `index.html` in your browser.
3. Every button press, throttle drag, and PID slider movement will transmit directly to your robot microcontroller over UDP!
