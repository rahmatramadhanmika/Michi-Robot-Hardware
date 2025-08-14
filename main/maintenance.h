#ifndef maintenance_h
#define maintenance_h

#include <Arduino.h>
#include <SD.h>
#include <Audio.h>
#include <PubSubClient.h>
#include "action.h"
#include "hands.h"
#include "neck.h"
#include "eyes.h"

class Maintenance {
public:
  Maintenance();
  void begin(Action* robotAction, Hands* handsPtr, Neck* neckPtr, eyes* eyesPtr, Audio* audioPtr, bool* audioFinishedPtr, PubSubClient* mqttPtr);
  void initializeAfterSD();  // Call this after SD card is initialized
  void checkSerialCommands();
  void processCommand(String command);  // Make this public for MQTT access
  void testHands();
  void testNeck();
  void testEyes();
  void testSpeaker();
  void testAll();
  void showHelp();
  void returnToSleepState();  // Helper function to return robot to sleep state
  void stopTest();            // Stop any running test
  void keepSystemResponsive();  // Helper to maintain MQTT processing during tests

private:
  Action* robot;
  Hands* hands;
  Neck* neck;
  eyes* eyesDisplay;
  Audio* audio;
  bool* audioFinished;
  PubSubClient* mqttClient;  // Add MQTT client for processing during maintenance
  bool maintenanceMode = false;
  volatile bool stopRequested = false; // Flag to request stop

  // Music files for speaker testing
  String musicFiles[20];
  int musicFileCount = 0;

  void loadMusicFiles();
};

#endif