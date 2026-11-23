#pragma once
#include <Arduino.h>
#include "config.h"
#include "robot_drive.h"

/* =============================================================================
 * SHIFTCTRLHUB — LFR PIDV LINE FOLLOWING CONTROLLER
 * =============================================================================
 * Implements:
 *   1. 5-Sensor Weighted Position Calculation ({-4, -2, 0, 2, 4})
 *   2. Real-time Tunable PIDV (Proportional, Integral, Derivative, Velocity Scaling)
 *   3. Intelligent Pattern Recovery for line loss and sharp 90-degree turns
 *      (Based on Abdullah Mian's tested LFR architecture)
 * ============================================================================= */

class LFRController {
private:
  RobotDrive* _drive;

  // LFR Parameters
  int   _baseSpeed = DEFAULT_LFR_SPEED;
  float _kp        = DEFAULT_LFR_KP;
  float _ki        = DEFAULT_LFR_KI;
  float _kd        = DEFAULT_LFR_KD;
  float _kv        = DEFAULT_LFR_KV;

  // State
  bool  _enabled   = false;
  float _lastError = 0.0f;
  float _integral  = 0.0f;
  String _lastPattern = "00100";
  int   _rawSensors[5] = {0, 0, 0, 0, 0};

  bool isSensorOnLine(int reading) {
    #if LINE_IS_BLACK
      return reading < IR_THRESHOLD;
    #else
      return reading >= IR_THRESHOLD;
    #endif
  }

public:
  LFRController(RobotDrive* drive) : _drive(drive) {}

  void begin() {
    pinMode(PIN_IR_S1, INPUT);
    pinMode(PIN_IR_S2, INPUT);
    pinMode(PIN_IR_S3, INPUT);
    pinMode(PIN_IR_S4, INPUT);
    pinMode(PIN_IR_S5, INPUT);
    _enabled = false;
    _integral = 0.0f;
    _lastError = 0.0f;
  }

  void setEnabled(bool enable) {
    _enabled = enable;
    if (!_enabled && _drive) {
      _drive->stop();
    }
    _integral = 0.0f;
    _lastError = 0.0f;
  }

  bool isEnabled() const { return _enabled; }

  // Parameter Setters
  void setBaseSpeed(int spd) { _baseSpeed = constrain(spd, 0, 255); }
  void setKp(float kp)       { _kp = max(0.0f, kp); }
  void setKi(float ki)       { _ki = max(0.0f, ki); }
  void setKd(float kd)       { _kd = max(0.0f, kd); }
  void setKv(float kv)       { _kv = max(0.0f, kv); }

  // Getters for telemetry
  int   getBaseSpeed() const { return _baseSpeed; }
  float getKp() const        { return _kp; }
  float getKi() const        { return _ki; }
  float getKd() const        { return _kd; }
  float getKv() const        { return _kv; }

  // Read all 5 sensors
  void readSensors() {
    _rawSensors[0] = analogRead(PIN_IR_S1);
    _rawSensors[1] = analogRead(PIN_IR_S2);
    _rawSensors[2] = analogRead(PIN_IR_S3);
    _rawSensors[3] = analogRead(PIN_IR_S4);
    _rawSensors[4] = analogRead(PIN_IR_S5);
  }

  // Main control cycle (invoked inside loop() when LFR mode is active)
  void update() {
    if (!_enabled || !_drive) return;

    readSensors();

    // 1. Build binary pattern and weighted position
    const int weights[5] = { -4, -2, 0, 2, 4 };
    int activeCount = 0;
    int position = 0;
    String currentPattern = "";

    for (int i = 0; i < 5; i++) {
      if (isSensorOnLine(_rawSensors[i])) {
        position += weights[i];
        activeCount++;
        currentPattern += "1";
      } else {
        currentPattern += "0";
      }
    }

    // 2. Line Loss Recovery (Abdullah Mian's recovery algorithm)
    if (activeCount == 0 || currentPattern == "00000") {
      recoverLineLoss();
      return;
    }

    // 3. PID Calculation
    float error = (float)position / (float)activeCount;
    _integral += error;
    _integral = constrain(_integral, -100.0f, 100.0f); // Anti-windup clamping
    float derivative = error - _lastError;
    _lastError = error;

    // Standard PID steering correction
    float correction = (_kp * error) + (_ki * _integral) + (_kd * derivative);

    // 4. Velocity Scaling (Kv damping for cornering)
    // As error grows, reduce forward speed to prevent overshooting sharp turns
    int effectiveSpeed = _baseSpeed - (int)(_kv * abs(error) * 12.0f);
    effectiveSpeed = constrain(effectiveSpeed, 40, 255);

    // Differential wheel speed computation
    int leftSpeed  = effectiveSpeed - (int)correction;
    int rightSpeed = effectiveSpeed + (int)correction;

    leftSpeed  = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    _drive->setMotorsRaw(leftSpeed, rightSpeed);

    // Save pattern for recovery lookback
    _lastPattern = currentPattern;
  }

private:
  // Recovers robot back onto line when all sensors read white
  void recoverLineLoss() {
    int recoverySpeed = constrain(_baseSpeed, 80, 180);

    // Check if line was on the Left
    if (_lastPattern == "11110" || _lastPattern == "11100" || 
        _lastPattern == "11000" || _lastPattern == "10000" || 
        _lastPattern == "01100") {
      // Rotate left until line reacquired
      _drive->spinLeft(recoverySpeed);
      waitForLineReacquisition();
    }
    // Check if line was on the Right
    else if (_lastPattern == "01111" || _lastPattern == "00111" || 
             _lastPattern == "00011" || _lastPattern == "00001" || 
             _lastPattern == "00110") {
      // Rotate right until line reacquired
      _drive->spinRight(recoverySpeed);
      waitForLineReacquisition();
    }
    // Center loss fallback
    else {
      _drive->spinLeft(recoverySpeed);
      waitForLineReacquisition();
    }
  }

  void waitForLineReacquisition() {
    unsigned long startTime = millis();
    while (millis() - startTime < 400) { // Max 400ms burst per check to avoid blocking
      readSensors();
      for (int i = 0; i < 5; i++) {
        if (isSensorOnLine(_rawSensors[i])) {
          _drive->stop();
          return;
        }
      }
      delay(2);
    }
  }
};
