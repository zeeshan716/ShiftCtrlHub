#pragma once
#include <Arduino.h>
#include "config.h"

/* =============================================================================
 * SHIFTCTRLHUB LFR — DIFFERENTIAL DRIVE MOTOR OUTPUT
 * ============================================================================= */

class RobotDrive {
private:
  int _masterSpeed = DEFAULT_MASTER_SPEED;

  void writeChannelPair(uint8_t pinFwd, uint8_t pinRev, int signedPwm) {
    signedPwm = constrain(signedPwm, -255, 255);
    if (signedPwm >= 0) {
      analogWrite(pinFwd, signedPwm);
      analogWrite(pinRev, 0);
    } else {
      analogWrite(pinFwd, 0);
      analogWrite(pinRev, -signedPwm);
    }
  }

  void writeMotors(int leftSignedPwm, int rightSignedPwm) {
    writeChannelPair(PIN_MOTOR_IN1, PIN_MOTOR_IN2, leftSignedPwm);
    writeChannelPair(PIN_MOTOR_IN3, PIN_MOTOR_IN4, rightSignedPwm);
  }

public:
  void begin() {
    const uint8_t motorPins[4] = {
      PIN_MOTOR_IN1, PIN_MOTOR_IN2, PIN_MOTOR_IN3, PIN_MOTOR_IN4
    };
    for (uint8_t i = 0; i < 4; i++) {
      pinMode(motorPins[i], OUTPUT);
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
      analogWriteFrequency(motorPins[i], MOTOR_PWM_FREQ);
      analogWriteResolution(motorPins[i], MOTOR_PWM_RES);
#endif
    }
    stop();
  }

  void setMasterSpeed(int speed) { _masterSpeed = constrain(speed, 0, 255); }
  int getMasterSpeed() const { return _masterSpeed; }

  void stop() {
    writeMotors(0, 0);
  }

  void setMotorsRaw(int leftPwm, int rightPwm) {
    writeMotors(constrain(leftPwm, 0, 255), constrain(rightPwm, 0, 255));
  }

  void spinLeft(int speed) {
    speed = constrain(speed, 0, 255);
    writeMotors(-speed, speed);
  }

  void spinRight(int speed) {
    speed = constrain(speed, 0, 255);
    writeMotors(speed, -speed);
  }
};
