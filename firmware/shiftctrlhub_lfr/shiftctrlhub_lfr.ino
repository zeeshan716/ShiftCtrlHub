/* =============================================================================
 * SHIFTCTRLHUB — DEDICATED LINE FOLLOWER (LFR PIDV) FIRMWARE
 * =============================================================================
 * Autonomous 5-sensor weighted PIDV line follower with live wireless parameter
 * tuning from ShiftCtrlHub (`V:Speed:*`, `V:Kp:*`, `V:Ki:*`, `V:Kd:*`, `V:Kv:*`,
 * `V:ALL:*`), plus `MODE:LFR` / `C:F` (Start) and `MODE:RC` / `C:S` (Pause/Stop).
 * ============================================================================= */

#include <Arduino.h>
#include "config.h"
#include "robot_drive.h"
#include "lfr_pidv.h"
#include "transports.h"

RobotDrive drive;
LFRController lfr(&drive);
TransportManager transport;

void processIncomingCommand(const String& cmd) {
  if (cmd.length() == 0) return;

  Serial.printf("[LFR CMD] -> %s\n", cmd.c_str());

  // 1. Start / Pause Controls
  if (cmd == "MODE:LFR" || cmd == "A" || cmd == "C:F") {
    lfr.setEnabled(true);
    Serial.println("[LFR] Autonomous Tracking STARTED");
    return;
  } else if (cmd == "MODE:RC" || cmd == "M" || cmd == "C:S") {
    lfr.setEnabled(false);
    drive.stop();
    Serial.println("[LFR] Autonomous Tracking PAUSED");
    return;
  }

  // 2. Real-Time PIDV Parameter Tuning (V:<param>:<val> or V:ALL:...)
  if (cmd.startsWith("V:")) {
    int firstColon = cmd.indexOf(':');
    int secondColon = cmd.indexOf(':', firstColon + 1);
    if (secondColon != -1) {
      String param = cmd.substring(firstColon + 1, secondColon);
      float val = cmd.substring(secondColon + 1).toFloat();

      if (param.equalsIgnoreCase("ALL")) {
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

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n========================================================");
  Serial.println("   SHIFTCTRLHUB — LFR-ONLY PIDV FIRMWARE (INITIALIZING)");
  Serial.println("========================================================");

  drive.begin();
  Serial.println("[INIT] Differential Drive Motors Initialized.");

  lfr.begin();
  Serial.println("[INIT] LFR 5-IR Sensor Array Initialized.");

  transport.begin();
  Serial.println("[INIT] Active Wireless Transport Initialized.");

  Serial.println("\n[ShiftCtrlHub LFR] Switch remote to LFR mode to start!\n");
}

void loop() {
  String incomingCmd = transport.checkIncoming();
  if (incomingCmd.length() > 0) {
    processIncomingCommand(incomingCmd);
  }

  if (lfr.isEnabled()) {
    lfr.update();
  }

  delay(1);
}
