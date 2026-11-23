/* =============================================================================
 * SHIFTCTRLHUB — DEDICATED RC CAR FIRMWARE (4WD + 2WD + JOYSTICK)
 * =============================================================================
 * Connects seamlessly to the ShiftCtrlHub interface over Wi-Fi (Hybrid/SoftAP/STA)
 * or Bluetooth (BLE/SPP) without any LFR sensor overhead.
 * ============================================================================= */

#include <Arduino.h>
#include "config.h"
#include "robot_drive.h"
#include "transports.h"

RobotDrive drive;
TransportManager transport;

enum DriveArchitecture {
  ARCH_4V4_DIFF,
  ARCH_2WD_STEER
};

DriveArchitecture currentArch = ARCH_4V4_DIFF;

void processIncomingCommand(const String& cmd) {
  if (cmd.length() == 0) return;

  Serial.printf("[RC CMD] -> %s\n", cmd.c_str());

  // 1. Drive Architecture Switching (4v4 vs 2WD)
  if (cmd == "DRIVE:4V4" || cmd == "DRIVE:4WD") {
    currentArch = ARCH_4V4_DIFF;
    drive.stop();
    drive.setSteerAngle(SERVO_CENTER_DEG);
    Serial.println("[STATE] Switched to 4v4 Differential-Drive");
    return;
  } else if (cmd == "DRIVE:2WD") {
    currentArch = ARCH_2WD_STEER;
    drive.stop();
    drive.setSteerAngle(SERVO_CENTER_DEG);
    Serial.println("[STATE] Switched to 2WD Front-Wheel Steer");
    return;
  }

  // 2. Motion Commands (C:F, C:B, C:L, C:R, C:FR, C:FL, C:BR, C:BL, C:S)
  if (cmd.startsWith("C:")) {
    String action = cmd.substring(2);
    if (currentArch == ARCH_4V4_DIFF) {
      if (action == "FR")      drive.drive4wdJoystick(50, 100);
      else if (action == "FL") drive.drive4wdJoystick(-50, 100);
      else if (action == "BR") drive.drive4wdJoystick(50, -100);
      else if (action == "BL") drive.drive4wdJoystick(-50, -100);
      else                     drive.drive4wdCardinal(action.charAt(0));
    } else {
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
    return;
  }

  // 3. Master Speed Throttle (S:<0-255>)
  if (cmd.startsWith("S:") && !cmd.startsWith("S:FB:") && !cmd.startsWith("S:RL:")) {
    int spd = cmd.substring(2).toInt();
    drive.setMasterSpeed(spd);
    Serial.printf("[DRIVE] Master Speed set to: %d\n", spd);
    return;
  }

  // 4. 2WD Dual Sliders (S:FB:<val> and S:RL:<val>)
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

  // 5. Joystick Vector (JOY:<x>:<y>)
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
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n========================================================");
  Serial.println("   SHIFTCTRLHUB — RC-ONLY ROBOT FIRMWARE (INITIALIZING)");
  Serial.println("========================================================");

  drive.begin();
  Serial.println("[INIT] Motors & Steering Servo Initialized.");

  transport.begin();
  Serial.println("[INIT] Active Wireless Transport Initialized.");

  Serial.println("\n[ShiftCtrlHub RC] Ready for Commands!\n");
}

void loop() {
  String incomingCmd = transport.checkIncoming();
  if (incomingCmd.length() > 0) {
    processIncomingCommand(incomingCmd);
  }
  delay(1);
}
