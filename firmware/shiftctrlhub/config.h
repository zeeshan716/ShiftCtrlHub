#pragma once
#include <Arduino.h>

/* =============================================================================
 * SHIFTCTRLHUB — UNIVERSAL FIRMWARE CONFIGURATION
 * =============================================================================
 * Modular Arduino-compatible Firmware for ShiftCtrlHub Remote.
 * Supports:
 *   1. Wi-Fi SoftAP (Direct Hotspot, 192.168.4.1, UDP port 3333)
 *   2. Wi-Fi STA with WiFiMulti (Multi-network auto-failover, UDP port 3333)
 *   3. BLE (Bluetooth Low Energy UART Service)
 *   4. Classic Bluetooth SPP (BluetoothSerial)
 * ============================================================================= */

// -----------------------------------------------------------------------------
// 1. TRANSPORT MODE SELECTION
// -----------------------------------------------------------------------------
#define CONN_SOFTAP  1   // Direct Access Point hosted by the robot MCU (UDP)
#define CONN_STA     2   // Local Wi-Fi Router / Station with WiFiMulti (UDP)
#define CONN_BLE     3   // Bluetooth Low Energy (BLE GATT Server)
#define CONN_SPP     4   // Bluetooth Classic SPP (SerialBT)

// CHOOSE ACTIVE TRANSPORT (Change here to select mode):
#define ACTIVE_TRANSPORT CONN_STA

// -----------------------------------------------------------------------------
// 2. WI-FI STA CREDENTIALS (WiFiMulti Auto-Connect List)
// Add as many networks as you need; the MCU will connect to the strongest
// available AP and automatically cycle between them if signal is lost.
// -----------------------------------------------------------------------------
struct WiFiCredential {
  const char* ssid;
  const char* password;
};

const WiFiCredential STA_NETWORKS[] = {
  { "YourWiFiSSID",                "YourWiFiPassword" },
  { "MyHomeWiFi",                  "HomePassword123" },
  { "PhoneHotspot",                "HotspotPass123" },
  { "LabRobotics",                 "LabSecret2026" }
};
const size_t STA_NETWORK_COUNT = sizeof(STA_NETWORKS) / sizeof(STA_NETWORKS[0]);

// -----------------------------------------------------------------------------
// 3. WI-FI SOFTAP CONFIGURATION
// -----------------------------------------------------------------------------
#define SOFTAP_SSID      "ShiftCtrlHub"
#define SOFTAP_PASS      "12345678"          // Minimum 8 characters
#define SOFTAP_CHANNEL   1
#define SOFTAP_MAX_CONN  4

// -----------------------------------------------------------------------------
// 4. UDP PORT & DISCOVERY CONFIGURATION
// -----------------------------------------------------------------------------
#define UDP_PORT              3333
#define DISCOVERY_REQ_TOKEN   "Discovery_Request"
#define HANDSHAKE_REQ_TOKEN   "Lets Talk"
#define HEARTBEAT_INTERVAL_MS 1000

// -----------------------------------------------------------------------------
// 5. BLUETOOTH CONFIGURATION (BLE & Classic SPP)
// -----------------------------------------------------------------------------
#define BT_DEVICE_NAME        "ShiftCtrlHub"

// Standard Nordic UART Service UUIDs (Compatible with ShiftCtrlHub BLE)
#define BLE_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// -----------------------------------------------------------------------------
// 6. MOTOR DRIVER HARDWARE PINS (L298N / TB6612FNG / DRV8833)
// 4WD Differential-Drive: Left motors on IN1/IN2, Right motors on IN3/IN4.
// Uses direct 4-pin analogWrite() PWM on IN1-IN4 (keep L298N ENA/ENB jumpers on).
// -----------------------------------------------------------------------------
#define PIN_MOTOR_IN1  18   // Left Motor Forward (analogWrite PWM)
#define PIN_MOTOR_IN2  19   // Left Motor Backward (analogWrite PWM)
#define PIN_MOTOR_IN3  22   // Right Motor Forward (analogWrite PWM)
#define PIN_MOTOR_IN4  23   // Right Motor Backward (analogWrite PWM)

#define MOTOR_PWM_FREQ   1000  // 1 kHz PWM frequency
#define MOTOR_PWM_RES    8     // 8-bit resolution (0-255)

// -----------------------------------------------------------------------------
// 7. 2WD STEERING SERVO PIN (SG90 / MG90S / Standard Servo)
// 2WD Toy-Car / Real-Car mode: Rear motor drives throttle, front servo turns wheels.
// -----------------------------------------------------------------------------
#define PIN_STEER_SERVO       21
#define SERVO_CENTER_DEG      90   // Center straight ahead
#define SERVO_MIN_DEG         45   // Max Left deflection
#define SERVO_MAX_DEG         135  // Max Right deflection

// -----------------------------------------------------------------------------
// 8. LINE FOLLOWER ROBOT (LFR) 5-IR SENSOR ARRAY PINS
// 5 IR reflective sensors (TCRT5000 / QTR-5A / digital comparator modules).
// -----------------------------------------------------------------------------
#define PIN_IR_S1      34   // Far Left
#define PIN_IR_S2      35   // Mid Left
#define PIN_IR_S3      32   // Center
#define PIN_IR_S4      33   // Mid Right
#define PIN_IR_S5      25   // Far Right

#define IR_THRESHOLD   1500  // Analog reading below this indicates line
#define LINE_IS_BLACK  true  // true = black line on white surface; false = white on black

// -----------------------------------------------------------------------------
// 9. DEFAULT ROBOT BEHAVIOR SETTINGS
// -----------------------------------------------------------------------------
#define DEFAULT_MASTER_SPEED 180   // Default RC drive throttle (0-255)
#define DEFAULT_LFR_SPEED    140   // Default LFR base cruise speed (0-255)
#define DEFAULT_LFR_KP       2.5f  // Proportional gain
#define DEFAULT_LFR_KI       0.05f // Integral gain
#define DEFAULT_LFR_KD       1.8f  // Derivative gain
#define DEFAULT_LFR_KV       1.2f  // Velocity scaling cornering damping factor
