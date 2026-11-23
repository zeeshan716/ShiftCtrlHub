#pragma once
#include <Arduino.h>
#include "config.h"

/* =============================================================================
 * SHIFTCTRLHUB — ROBOT DRIVE CONTROLLER
 * =============================================================================
 * Supports:
 *   1. 4WD Differential-Drive / Skid-Steer (Tank style, 4 cardinal commands & mixed joystick)
 *   2. 2WD Front-Wheel Steer (Toy-car style, DC drive motor throttle + Steering Servo)
 *   3. Raw differential output for LFR PIDV autonomous mode
 * ============================================================================= */

class RobotDrive {
private:
  int _masterSpeed = DEFAULT_MASTER_SPEED;
  int _currentServoAngle = SERVO_CENTER_DEG;

  // Drive one H-bridge channel pair (IN_A/IN_B) with signed 8-bit PWM (-255..255).
  // Matches 4-pin L298N wiring where ENA/ENB jumpers stay installed.
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

  void writeLeftMotor(int signedPwm) {
    writeChannelPair(PIN_MOTOR_IN1, PIN_MOTOR_IN2, signedPwm);
  }

  void writeRightMotor(int signedPwm) {
    writeChannelPair(PIN_MOTOR_IN3, PIN_MOTOR_IN4, signedPwm);
  }

  void writeMotors(int leftSignedPwm, int rightSignedPwm) {
    writeLeftMotor(leftSignedPwm);
    writeRightMotor(rightSignedPwm);
  }

  // Servo 50Hz PWM output using Arduino analogWrite()
  void writeServoMicros(int us) {
    // 50Hz period = 20,000us; 16-bit timer resolution maps duty across 0..65535
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
    uint32_t duty = (uint32_t)(((float)us / 20000.0f) * 65535.0f);
    analogWrite(PIN_STEER_SERVO, duty);
#else
    uint8_t duty8 = (uint8_t)constrain(((us * 255L) / 20000L), 0, 255);
    analogWrite(PIN_STEER_SERVO, duty8);
#endif
  }

public:
  void begin() {
    // Configure 4 motor IN pins for direct analogWrite() PWM
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

    // Configure Steering Servo PWM via analogWrite() (50 Hz, 16-bit on Core 3+)
    pinMode(PIN_STEER_SERVO, OUTPUT);
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
    analogWriteFrequency(PIN_STEER_SERVO, 50);
    analogWriteResolution(PIN_STEER_SERVO, 16);
#endif

    stop();
    setSteerAngle(SERVO_CENTER_DEG);
  }

  void setMasterSpeed(int speed) {
    _masterSpeed = constrain(speed, 0, 255);
  }

  int getMasterSpeed() const {
    return _masterSpeed;
  }

  void stop() {
    writeMotors(0, 0);
  }

  // Raw forward differential drive (used by LFR PID)
  void setMotorsRaw(int leftPwm, int rightPwm) {
    writeMotors(constrain(leftPwm, 0, 255), constrain(rightPwm, 0, 255));
  }

  // Raw differential spin (used by LFR recovery)
  void spinLeft(int speed) {
    speed = constrain(speed, 0, 255);
    writeMotors(-speed, speed);
  }

  void spinRight(int speed) {
    speed = constrain(speed, 0, 255);
    writeMotors(speed, -speed);
  }

  // ---------------------------------------------------------------------------
  // 4WD DIFFERENTIAL-DRIVE (Skid-Steer / Tank Mode)
  // ---------------------------------------------------------------------------
  void drive4wdCardinal(char dir) {
    int spd = _masterSpeed;
    switch (dir) {
      case 'F': // Forward
        writeMotors(spd, spd);
        break;

      case 'B': // Backward
        writeMotors(-spd, -spd);
        break;

      case 'L': // Spin Left on axis
        writeMotors(-spd, spd);
        break;

      case 'R': // Spin Right on axis
        writeMotors(spd, -spd);
        break;

      case 'S': // Stop
      default:
        stop();
        break;
    }
  }

  // 4WD Joystick Mixing (X = -100 to +100, Y = -100 to +100)
  void drive4wdJoystick(int xPercent, int yPercent) {
    if (abs(xPercent) < 5 && abs(yPercent) < 5) {
      stop();
      return;
    }

    // Convert percentages to speed components
    float forward = ((float)yPercent / 100.0f) * (float)_masterSpeed;
    float turn = ((float)xPercent / 100.0f) * (float)_masterSpeed;

    int left = constrain((int)(forward + turn), -255, 255);
    int right = constrain((int)(forward - turn), -255, 255);

    writeMotors(left, right);
  }

  // ---------------------------------------------------------------------------
  // 2WD FRONT-WHEEL STEER (Toy-Car / Servo Steer Mode)
  // ---------------------------------------------------------------------------
  void setSteerAngle(int angleDeg) {
    _currentServoAngle = constrain(angleDeg, SERVO_MIN_DEG, SERVO_MAX_DEG);
    // Map angle (0 to 180) to microseconds (500us to 2500us)
    int us = map(_currentServoAngle, 0, 180, 500, 2500);
    writeServoMicros(us);
  }

  // Steer percentage (-100% = Full Left, 0% = Straight, +100% = Full Right)
  void steer2wdPercent(int steerPercent) {
    steerPercent = constrain(steerPercent, -100, 100);
    int angle = map(steerPercent, -100, 100, SERVO_MIN_DEG, SERVO_MAX_DEG);
    setSteerAngle(angle);
  }

  // 2WD Throttle percentage (-100% Reverse to +100% Forward)
  void drive2wdThrottle(int throttlePercent) {
    throttlePercent = constrain(throttlePercent, -100, 100);

    if (abs(throttlePercent) < 3) {
      // Rear motor stop
      writeLeftMotor(0);
      return;
    }

    int pwm = (int)(((float)abs(throttlePercent) / 100.0f) * (float)_masterSpeed);
    pwm = constrain(pwm, 0, 255);
    writeLeftMotor(throttlePercent > 0 ? pwm : -pwm);
  }
};
