/* =============================================================================
 * SHIFTCTRLHUB — UNIVERSAL ROBOTICS FIRMWARE
 * =============================================================================
 * Complete, production-ready firmware matching the ShiftCtrlHub interface.
 *
 * Target Hardware: Multi-MCU Arduino Framework (Reference pins: ESP32)
 *
 * Key Capabilities:
 *  1. 4 Wireless Transport Modes:
 *     - Wi-Fi Station (STA) with WiFiMulti multi-AP auto-failover (UDP port 3333)
 *     - Wi-Fi SoftAP (Direct AP at 192.168.4.1, UDP port 3333)
 *     - Bluetooth Low Energy (BLE GATT UART Server)
 *     - Classic Bluetooth SPP (BluetoothSerial)
 *  2. Dual Robot Drive Architecture:
 *     - 4v4: 4WD Differential-Drive / Skid-Steer (4 cardinal directions & joystick mixing)
 *     - 2WD: Front-Wheel Steer Vehicle (rear DC throttle + front steering servo)
 *  3. LFR PIDV Line Follower:
 *     - 5-IR reflective sensor array with weighted position error
 *     - Real-time live parameters (Base Speed, Kp, Ki, Kd, and Kv corner damping)
 *     - Intelligent pattern-based line loss recovery & 90-degree turn assistance
 * ============================================================================= */

#include <Arduino.h>
#include "config.h"
#include "robot_drive.h"
#include "lfr_pidv.h"
#include "transports.h"

// -----------------------------------------------------------------------------
// GLOBAL CONTROLLERS & STATE
// -----------------------------------------------------------------------------
RobotDrive drive;
LFRController lfr(&drive);
TransportManager transport;

enum RobotOperationalMode {
  OP_MODE_RC,
  OP_MODE_LFR
};

enum DriveArchitecture {
  ARCH_4V4_DIFF,
  ARCH_2WD_STEER
};

RobotOperationalMode currentMode = OP_MODE_RC;
DriveArchitecture currentArch = ARCH_4V4_DIFF;

// -----------------------------------------------------------------------------
// COMMAND DISPATCH & PARSER
// -----------------------------------------------------------------------------
void processIncomingCommand(const String& cmd) {
  if (cmd.length() == 0) return;

  Serial.printf("[CMD RX] -> %s\n", cmd.c_str());

  // 1. Operational Mode Switching
  if (cmd == "MODE:RC" || cmd == "M") {
    currentMode = OP_MODE_RC;
    lfr.setEnabled(false);
    drive.stop();
    Serial.println("[STATE] Switched to RC Mode");
    return;
  }
  else if (cmd == "MODE:LFR" || cmd == "A") {
    currentMode = OP_MODE_LFR;
    lfr.setEnabled(true);
    Serial.println("[STATE] Switched to LFR Mode");
    return;
  }

  // 2. Drive Architecture Switching (4v4 vs 2WD)
  if (cmd == "DRIVE:4V4" || cmd == "DRIVE:4WD") {
    currentArch = ARCH_4V4_DIFF;
    drive.stop();
    drive.setSteerAngle(SERVO_CENTER_DEG);
    Serial.println("[STATE] Switched to 4v4 Differential-Drive");
    return;
  }
  else if (cmd == "DRIVE:2WD") {
    currentArch = ARCH_2WD_STEER;
    drive.stop();
    drive.setSteerAngle(SERVO_CENTER_DEG);
    Serial.println("[STATE] Switched to 2WD Front-Wheel Steer");
    return;
  }

  // 3. Motion Commands (C:F, C:B, C:L, C:R, C:FR, C:FL, C:BR, C:BL, C:S)
  if (cmd.startsWith("C:")) {
    String action = cmd.substring(2);
    if (currentMode == OP_MODE_RC) {
      if (currentArch == ARCH_4V4_DIFF) {
        if (action == "FR")      drive.drive4wdJoystick(50, 100);
        else if (action == "FL") drive.drive4wdJoystick(-50, 100);
        else if (action == "BR") drive.drive4wdJoystick(50, -100);
        else if (action == "BL") drive.drive4wdJoystick(-50, -100);
        else                     drive.drive4wdCardinal(action.charAt(0));
      } else {
        // 2WD buttons & directional joystick vectors
        if (action == "F")       { drive.drive2wdThrottle(100);  drive.steer2wdPercent(0); }
        else if (action == "B")  { drive.drive2wdThrottle(-100); drive.steer2wdPercent(0); }
        else if (action == "FR") { drive.drive2wdThrottle(100);  drive.steer2wdPercent(75); }
        else if (action == "FL") { drive.drive2wdThrottle(100);  drive.steer2wdPercent(-75); }
        else if (action == "BR") { drive.drive2wdThrottle(-100); drive.steer2wdPercent(75); }
        else if (action == "BL") { drive.drive2wdThrottle(-100); drive.steer2wdPercent(-75); }
        else if (action == "L")  { drive.drive2wdThrottle(60);   drive.steer2wdPercent(-80); }
        else if (action == "R")  { drive.drive2wdThrottle(60);   drive.steer2wdPercent(80); }
        else                     { drive.drive2wdThrottle(0);    drive.steer2wdPercent(0); }
      }
    } else {
      // In LFR mode, C:S pauses line follower
      if (action == "S") {
        lfr.setEnabled(false);
      } else if (action == "F") {
        lfr.setEnabled(true);
      }
    }
    return;
  }

  // 4. Master Speed Throttle (S:<val>)
  if (cmd.startsWith("S:") && !cmd.startsWith("S:FB:") && !cmd.startsWith("S:RL:")) {
    int spd = cmd.substring(2).toInt();
    drive.setMasterSpeed(spd);
    Serial.printf("[DRIVE] Master Speed set to: %d\n", spd);
    return;
  }

  // 5. 2WD Dual Sliders (S:FB:<val> and S:RL:<val>)
  if (cmd.startsWith("S:FB:")) {
    int throttleVal = cmd.substring(5).toInt();
    if (currentArch == ARCH_2WD_STEER) {
      drive.drive2wdThrottle(throttleVal);
    }
    return;
  }
  if (cmd.startsWith("S:RL:")) {
    int steerVal = cmd.substring(5).toInt();
    if (currentArch == ARCH_2WD_STEER) {
      drive.steer2wdPercent(steerVal);
    }
    return;
  }

  // 6. Joystick Polar / Cartesian (JOY:<x>:<y>)
  if (cmd.startsWith("JOY:")) {
    int firstColon = cmd.indexOf(':');
    int secondColon = cmd.indexOf(':', firstColon + 1);
    if (secondColon != -1) {
      int xVal = cmd.substring(firstColon + 1, secondColon).toInt();
      int yVal = cmd.substring(secondColon + 1).toInt();
      if (currentArch == ARCH_4V4_DIFF) {
        drive.drive4wdJoystick(xVal, yVal);
      } else {
        drive.drive2wdThrottle(yVal);
        drive.steer2wdPercent(xVal);
      }
    }
    return;
  }

  // 7. LFR Real-Time PIDV Parameter Tuning (V:<param>:<val>)
  if (cmd.startsWith("V:")) {
    int firstColon = cmd.indexOf(':');
    int secondColon = cmd.indexOf(':', firstColon + 1);
    if (secondColon != -1) {
      String param = cmd.substring(firstColon + 1, secondColon);
      float val = cmd.substring(secondColon + 1).toFloat();

      if (param.equalsIgnoreCase("ALL")) {
        // Format: V:ALL:<speed>:<kp>:<ki>:<kd>:<kv>
        int c3 = cmd.indexOf(':', secondColon + 1);
        int c4 = (c3 != -1) ? cmd.indexOf(':', c3 + 1) : -1;
        int c5 = (c4 != -1) ? cmd.indexOf(':', c4 + 1) : -1;
        int c6 = (c5 != -1) ? cmd.indexOf(':', c5 + 1) : -1;
        if (c6 != -1) {
          lfr.setBaseSpeed(cmd.substring(secondColon + 1, c3).toInt());
          lfr.setKp(cmd.substring(c3 + 1, c4).toFloat());
          lfr.setKi(cmd.substring(c4 + 1, c5).toFloat());
          lfr.setKd(cmd.substring(c5 + 1, c6).toFloat());
          lfr.setKv(cmd.substring(c6 + 1).toFloat());
          Serial.printf("[LFR] ALL -> Spd:%d Kp:%.2f Ki:%.3f Kd:%.2f Kv:%.2f\n",
                        lfr.getBaseSpeed(), lfr.getKp(), lfr.getKi(), lfr.getKd(), lfr.getKv());
        }
      } else if (param.equalsIgnoreCase("Speed")) {
        lfr.setBaseSpeed((int)val);
        Serial.printf("[LFR] Base Speed -> %d\n", (int)val);
      } else if (param.equalsIgnoreCase("Kp")) {
        lfr.setKp(val);
        Serial.printf("[LFR] Kp -> %.3f\n", val);
      } else if (param.equalsIgnoreCase("Ki")) {
        lfr.setKi(val);
        Serial.printf("[LFR] Ki -> %.4f\n", val);
      } else if (param.equalsIgnoreCase("Kd")) {
        lfr.setKd(val);
        Serial.printf("[LFR] Kd -> %.3f\n", val);
      } else if (param.equalsIgnoreCase("Kv")) {
        lfr.setKv(val);
        Serial.printf("[LFR] Kv -> %.3f\n", val);
      }
    }
    return;
  }
}

// -----------------------------------------------------------------------------
// ARDUINO MAIN SETUP
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n========================================================");
  Serial.println("   SHIFTCTRLHUB — UNIVERSAL ROBOT FIRMWARE (INITIALIZING)");
  Serial.println("========================================================");

  // 1. Initialize Drive Hardware (analogWrite Motor PWM & Steering Servo)
  drive.begin();
  Serial.println("[INIT] Motors & Steering Servo Initialized.");

  // 2. Initialize LFR 5-IR Sensor Array
  lfr.begin();
  Serial.println("[INIT] LFR Sensor Array Initialized.");

  // 3. Initialize Selected Wireless Transport (STA / SoftAP / BLE / SPP)
  transport.begin();
  Serial.println("[INIT] Active Wireless Transport Initialized.");

  Serial.println("\n[ShiftCtrlHub] Robot is Ready for Commands!\n");
}

// -----------------------------------------------------------------------------
// ARDUINO MAIN LOOP
// -----------------------------------------------------------------------------
void loop() {
  // 1. Poll active wireless transport for incoming command
  String incomingCmd = transport.checkIncoming();
  if (incomingCmd.length() > 0) {
    processIncomingCommand(incomingCmd);
  }

  // 2. Autonomous LFR Control Loop
  if (currentMode == OP_MODE_LFR && lfr.isEnabled()) {
    lfr.update();
  }

  // Brief yield for RTOS scheduler stability
  delay(1);
}
