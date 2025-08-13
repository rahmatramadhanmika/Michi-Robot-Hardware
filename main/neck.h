#ifndef neck_h
#define neck_h

#include <Arduino.h>
#include <ESP32Servo.h>

// Forward declaration
class eyes;

class Neck {
public:
  Neck(int servoPin = 4);
  void begin();
  void setEyesReference(eyes* eyesPtr);  // Add method to set eyes reference
  void idle();
  void answer();
  void sad();
  void happy();
  void standBy();
  void detectWakeword();
  void thinking();
  void resetState(); // Add method to reset state variables
  void testNeck();

private:
  Servo servo1;
  int servoPin;
  int posDegrees = 80;
  bool increasing = true;
  unsigned long previousMillis = 0;
  const long detectInterval = 10;
  const long interval = 15;
  const long answerInterval = 25;
  const long sadInterval = 35;
  int targetDegrees = 80;
  bool waitingAtTarget = false;
  bool returningToCenter = false;
  unsigned long waitStartMillis = 0;
  unsigned long lastMoveMillis = 0;
  unsigned long lastUpdateMillis = 0;
  bool movingTo110 = true; // For sad mode: true for 110, false for 80
  eyes* eyesPtr = nullptr; // Pointer to eyes object
};

#endif