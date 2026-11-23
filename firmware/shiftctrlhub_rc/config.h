#pragma once
#include <Arduino.h>

/* =============================================================================
 * SHIFTCTRLHUB RC-ONLY — FIRMWARE CONFIGURATION
 * =============================================================================
 * Dedicated RC Car Firmware (4WD Differential-Drive & 2WD Front-Wheel Steer).
 * Supports Hybrid Auto-Wi-Fi (STA -> SoftAP), SoftAP, STA, BLE, and Classic SPP.
 * ============================================================================= */

// 1. TRANSPORT MODE SELECTION
#define CONN_SOFTAP  1   // Direct Access Point hosted by the robot MCU (UDP)
#define CONN_STA     2   // Local Wi-Fi Router / Station with WiFiMulti (UDP)
#define CONN_BLE     3   // Bluetooth Low Energy (BLE GATT Server)
#define CONN_SPP     4   // Bluetooth Classic SPP (SerialBT)

#define ACTIVE_TRANSPORT CONN_SOFTAP

// 1B. HYBRID AUTO-WIFI TOGGLE (1 = Try STA for 10s first, SoftAP fallback; 0 = Strict single mode)
#define ENABLE_HYBRID_WIFI    1
#define HYBRID_STA_TIMEOUT_MS 10000UL

// 2. WI-FI STA CREDENTIALS (WiFiMulti Auto-Connect List)
struct WiFiCredential {
  const char* ssid;
  const char* password;
};

const WiFiCredential STA_NETWORKS[] = {
  { "YourWiFiSSID", "YourWiFiPassword" },
  { "MyHomeWiFi",   "HomePassword123" },
  { "PhoneHotspot", "HotspotPass123" }
};
const size_t STA_NETWORK_COUNT = sizeof(STA_NETWORKS) / sizeof(STA_NETWORKS[0]);

// 3. WI-FI SOFTAP CONFIGURATION
#define SOFTAP_SSID      "ShiftCtrlHub"
#define SOFTAP_PASS      "12345678"
#define SOFTAP_CHANNEL   1
#define SOFTAP_MAX_CONN  4

// 4. UDP PORT & DISCOVERY CONFIGURATION
#define UDP_PORT              3333
#define DISCOVERY_REQ_TOKEN   "Discovery_Request"
#define HANDSHAKE_REQ_TOKEN   "Lets Talk"
#define HEARTBEAT_INTERVAL_MS 1000

// 5. BLUETOOTH CONFIGURATION (BLE & Classic SPP)
#define BT_DEVICE_NAME          "ShiftCtrlHub"
#define BLE_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// 6. MOTOR DRIVER HARDWARE PINS (L298N / TB6612FNG / DRV8833)
#define PIN_MOTOR_IN1  18   // Left Motor Forward (analogWrite PWM)
#define PIN_MOTOR_IN2  19   // Left Motor Backward (analogWrite PWM)
#define PIN_MOTOR_IN3  22   // Right Motor Forward (analogWrite PWM)
#define PIN_MOTOR_IN4  23   // Right Motor Backward (analogWrite PWM)

#define MOTOR_PWM_FREQ   1000
#define MOTOR_PWM_RES    8

// 7. 2WD STEERING SERVO PIN (SG90 / MG90S)
#define PIN_STEER_SERVO  21
#define SERVO_CENTER_DEG 90
#define SERVO_MIN_DEG    45
#define SERVO_MAX_DEG    135

// 8. DEFAULT RC SPEED
#define DEFAULT_MASTER_SPEED 180
