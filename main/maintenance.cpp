#include "maintenance.h"

Maintenance::Maintenance() {
}

void Maintenance::stopTest() {
  stopRequested = true;
  robot->standBy();
  Serial.println("=== STOP REQUESTED ===");
}

void Maintenance::begin(Action *robotAction, Hands *handsPtr, Neck *neckPtr, eyes *eyesPtr, Audio *audioPtr, bool *audioFinishedPtr, PubSubClient *mqttPtr) {
  robot = robotAction;
  hands = handsPtr;
  neck = neckPtr;
  eyesDisplay = eyesPtr;
  audio = audioPtr;
  audioFinished = audioFinishedPtr;
  mqttClient = mqttPtr;

  Serial.println("=== MAINTENANCE MODE READY ===");
  Serial.println("Available commands:");
  showHelp();
}

void Maintenance::initializeAfterSD() {
  // Load music files from cut folder for speaker testing
  loadMusicFiles();
}

void Maintenance::checkSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    if (command.length() > 0) {
      Serial.println("Received command: " + command);
      processCommand(command);
    }
  }
}

void Maintenance::processCommand(String command) {
  Serial.printf("DEBUG: processCommand() called with: %s\n", command.c_str());

  // Handle both formats: "test_hands" and "hands" (for MQTT compatibility)
  if (command == "test_hands" || command == "hands") {
    Serial.println("DEBUG: Starting hands test");
    testHands();
    returnToSleepState();
    Serial.println("DEBUG: Hands test completed");
  } else if (command == "test_neck" || command == "neck") {
    Serial.println("DEBUG: Starting neck test");
    testNeck();
    returnToSleepState();
    Serial.println("DEBUG: Neck test completed");
  } else if (command == "test_eyes" || command == "eyes") {
    Serial.println("DEBUG: Starting eyes test");
    testEyes();
    returnToSleepState();
    Serial.println("DEBUG: Eyes test completed");
  } else if (command == "test_speaker" || command == "speaker") {
    Serial.println("DEBUG: Starting speaker test");
    testSpeaker();
    returnToSleepState();
    Serial.println("DEBUG: Speaker test completed");
  } else if (command == "test_all" || command == "all") {
    Serial.println("DEBUG: Starting all tests");
    testAll();
    returnToSleepState();
    Serial.println("DEBUG: All tests completed");
  } else if (command == "help") {
    showHelp();
  } else if (command == "stop") {
    Serial.println("Stopping current test...");
    stopTest();
    robot->standBy();
  } else {
    Serial.println("Unknown command: " + command);
    Serial.println("Type 'help' for available commands");
  }

  Serial.printf("DEBUG: processCommand() finished for: %s\n", command.c_str());
}

void Maintenance::loadMusicFiles() {
  const char *musicFolder = "/cut";
  musicFileCount = 0;

  File dir = SD.open(musicFolder);
  if (!dir) {
    Serial.printf("Failed to open %s directory for speaker testing\n", musicFolder);
    return;
  }

  while (true) {
    File entry = dir.openNextFile();
    if (!entry)
      break;

    if (!entry.isDirectory()) {
      String fileName = entry.name();
      if (fileName.endsWith(".mp3") || fileName.endsWith(".wav")) {
        musicFiles[musicFileCount] = String(musicFolder) + "/" + fileName;
        musicFileCount++;
        if (musicFileCount >= 20)
          break;  // Limit to prevent array overflow
      }
    }
    entry.close();
  }
  dir.close();

  Serial.printf("Loaded %d music files from %s for speaker testing\n", musicFileCount, musicFolder);
}

void Maintenance::testHands() {
  stopRequested = false;
  Serial.println("=== TESTING HANDS SERVOS ===");
  Serial.println("Testing left and right hand servos individually...");

  // Test sequence: standby -> idle -> happy -> mad -> sad -> dance -> answer -> hearingQuestion -> standby
  Serial.println("1. Setting hands to standby position...");
  hands->standBy();
  delay(3000);

  Serial.println("2. Testing hands idle movement...");
  unsigned long startTime = millis();
  while (millis() - startTime < 8000) {  // 8 seconds
    if (stopRequested) return;
    hands->idle();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("3. Testing hands happy movement...");
  startTime = millis();
  while (millis() - startTime < 5000) {  // 5 seconds
    if (stopRequested) return;
    hands->happy();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("4. Testing hands mad movement...");
  startTime = millis();
  while (millis() - startTime < 4000) {  // 4 seconds
    if (stopRequested) return;
    hands->mad();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("5. Testing hands sad movement...");
  startTime = millis();
  while (millis() - startTime < 4000) {  // 4 seconds
    if (stopRequested) return;
    hands->sad();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("6. Testing hands dance movement...");
  startTime = millis();
  while (millis() - startTime < 6000) {  // 6 seconds
    if (stopRequested) return;
    hands->dance();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("7. Testing hands answer movement...");
  startTime = millis();
  while (millis() - startTime < 5000) {  // 5 seconds
    if (stopRequested) return;
    hands->answer();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("8. Testing hands hearingQuestion movement...");
  startTime = millis();
  while (millis() - startTime < 3000) {  // 3 seconds
    if (stopRequested) return;
    hands->hearingQuestion();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("9. Returning hands to standby...");
  hands->standBy();
  delay(2000);

  Serial.println("=== HANDS TEST COMPLETED ===");
  Serial.println("Check if both hand servos moved correctly through all positions");
  Serial.println("Left servo should have moved through various positions");
  Serial.println("Right servo should have moved through various positions");
  Serial.println("Robot ready for next maintenance command...");
  Serial.println();
}

void Maintenance::testNeck() {
  stopRequested = false;
  Serial.println("=== TESTING NECK SERVO ===");
  Serial.println("Testing neck servo movements individually...");

  Serial.println("1. Setting neck to standby position (80°)...");
  neck->standBy();
  delay(3000);

  Serial.println("2. Testing neck idle movement...");
  neck->resetState();  // Reset state before starting
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) {  // 10 seconds
    if (stopRequested) return;
    neck->idle();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("3. Testing neck answer movement...");
  neck->resetState();
  startTime = millis();
  while (millis() - startTime < 8000) {  // 8 seconds
    if (stopRequested) return;
    neck->answer();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("4. Testing neck sad movement...");
  neck->resetState();
  startTime = millis();
  while (millis() - startTime < 6000) {  // 6 seconds
    if (stopRequested) return;
    neck->sad();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("5. Testing neck happy movement...");
  neck->resetState();
  startTime = millis();
  while (millis() - startTime < 6000) {  // 6 seconds
    if (stopRequested) return;
    neck->happy();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("6. Testing neck thinking movement...");
  neck->resetState();
  startTime = millis();
  while (millis() - startTime < 10000) {  // 10 seconds
    if (stopRequested) return;
    neck->thinking();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("7. Testing neck detectWakeword movement...");
  neck->resetState();
  startTime = millis();
  while (millis() - startTime < 8000) {  // 8 seconds
    if (stopRequested) return;
    neck->detectWakeword();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("8. Testing neck testNeck function...");
  neck->resetState();
  startTime = millis();
  while (millis() - startTime < 8000) {  // 8 seconds
    if (stopRequested) return;
    neck->testNeck();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(10);
  }

  Serial.println("9. Returning neck to standby (80°)...");
  neck->standBy();
  delay(3000);

  Serial.println("=== NECK TEST COMPLETED ===");
  Serial.println("Check if neck servo moved correctly through all positions:");
  Serial.println("- Idle: Random positions 30-150°, return to 80°");
  Serial.println("- Answer: Random positions 60-120°, return to 80°");
  Serial.println("- Sad: Alternating 110° and 80°");
  Serial.println("- Happy: Random positions 60-120°, return to 80°");
  Serial.println("- Thinking: Random positions 30-150°, return to 80°");
  Serial.println("- DetectWakeword: Random 120° or 40°, return to 80°");
  Serial.println("- TestNeck: Random positions 0-180°");
  Serial.println("Robot ready for next maintenance command...");
  Serial.println();
}

void Maintenance::testEyes() {
  stopRequested = false;
  Serial.println("=== TESTING EYES DISPLAY ===");
  Serial.println("Testing OLED eye display individually...");

  Serial.println("1. Testing sleep eyes...");
  eyesDisplay->cancelExpression();
  eyesDisplay->sleep();
  for (int i = 0; i < 30; i++) {
    if (stopRequested) return;
    keepSystemResponsive();
    delay(100);
  }

  Serial.println("2. Testing center/normal eyes...");
  eyesDisplay->cancelExpression();
  eyesDisplay->centerEyes();
  for (int i = 0; i < 20; i++) {
    if (stopRequested) return;
    keepSystemResponsive();
    delay(100);
  }

  Serial.println("3. Testing wakeup animation...");
  eyesDisplay->wakeup();
  for (int i = 0; i < 30; i++) {
    if (stopRequested) return;
    keepSystemResponsive();
    delay(100);
  }

  Serial.println("4. Testing question eyes (static)...");
  eyesDisplay->cancelExpression();
  eyesDisplay->questioning();
  for (int i = 0; i < 30; i++) {
    if (stopRequested) return;
    keepSystemResponsive();
    delay(100);
  }

  Serial.println("5. Testing question animation...");
  eyesDisplay->cancelExpression();
  eyesDisplay->startQuestion();
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {  // 5 seconds
    if (stopRequested) return;
    eyesDisplay->updateQuestion();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("6. Testing happy animation...");
  eyesDisplay->cancelExpression();
  eyesDisplay->startHappy();
  startTime = millis();
  while (millis() - startTime < 4000) {  // 4 seconds
    if (stopRequested) return;
    eyesDisplay->updateHappy();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("7. Testing mad animation...");
  eyesDisplay->cancelExpression();
  eyesDisplay->startMad();
  startTime = millis();
  while (millis() - startTime < 4000) {  // 4 seconds
    if (stopRequested) return;
    eyesDisplay->updateMad();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("8. Testing sad animation...");
  eyesDisplay->cancelExpression();
  eyesDisplay->startSad();
  startTime = millis();
  while (millis() - startTime < 4000) {  // 4 seconds
    if (stopRequested) return;
    eyesDisplay->updateSad();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("9. Testing excited animation...");
  eyesDisplay->cancelExpression();
  eyesDisplay->startExcited();
  startTime = millis();
  while (millis() - startTime < 4000) {  // 4 seconds
    if (stopRequested) return;
    eyesDisplay->updateExcited();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("10. Testing bored animation...");
  eyesDisplay->cancelExpression();
  eyesDisplay->startBored();
  startTime = millis();
  while (millis() - startTime < 4000) {  // 4 seconds
    if (stopRequested) return;
    eyesDisplay->updateBored();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("11. Testing idle animation...");
  eyesDisplay->cancelExpression();
  eyesDisplay->startIdle();
  startTime = millis();
  while (millis() - startTime < 6000) {  // 6 seconds
    if (stopRequested) return;
    eyesDisplay->updateIdle();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("12. Testing big eye movement (left)...");
  eyesDisplay->cancelExpression();
  eyesDisplay->centerEyes();
  eyesDisplay->startMoveBigEye(-1);  // Move left
  startTime = millis();
  while (millis() - startTime < 3000) {  // 3 seconds
    if (stopRequested) return;
    eyesDisplay->updateMoveBigEye();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("13. Testing big eye movement (right)...");
  eyesDisplay->startMoveBigEye(1);  // Move right
  startTime = millis();
  while (millis() - startTime < 3000) {  // 3 seconds
    if (stopRequested) return;
    eyesDisplay->updateMoveBigEye();
    eyesDisplay->updateBlink();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
  }

  Serial.println("14. Testing manual blink...");
  eyesDisplay->cancelExpression();
  eyesDisplay->centerEyes();
  for (int i = 0; i < 5; i++) {  // 5 blinks
    if (stopRequested) return;
    eyesDisplay->startBlink(5);  // Fast blink
    startTime = millis();
    while (millis() - startTime < 800) {  // Wait for blink to complete
      if (stopRequested) return;
      eyesDisplay->updateBlink();
      keepSystemResponsive();  // Keep MQTT processing active
      delay(50);
    }
    delay(500);              // Pause between blinks
    keepSystemResponsive();  // Keep MQTT processing active during pause
  }

  Serial.println("15. Returning to center eyes...");
  eyesDisplay->cancelExpression();
  eyesDisplay->centerEyes();
  for (int i = 0; i < 20; i++) {
    if (stopRequested) return;
    keepSystemResponsive();
    delay(100);
  }

  keepSystemResponsive();
  Serial.println("=== EYES TEST COMPLETED ===");
  Serial.println("Check if OLED display showed all eye expressions correctly:");
  Serial.println("- Sleep: Eyes closed to thin lines");
  Serial.println("- Center: Normal round eyes");
  Serial.println("- Wakeup: Animation from sleep to open");
  Serial.println("- Question: Static question mark with eyes");
  Serial.println("- Question Animation: Smooth transition to question mark");
  Serial.println("- Happy: Triangle overlay animation");
  Serial.println("- Mad: Angry triangles moving down");
  Serial.println("- Sad: Sad triangles moving down");
  Serial.println("- Excited: Moving circles animation");
  Serial.println("- Bored: Rectangle overlay animation");
  Serial.println("- Idle: Random blinking patterns");
  Serial.println("- Big Eye Movement: Eyes moving left and right");
  Serial.println("- Manual Blink: Controlled blinking");
  Serial.println("Robot ready for next maintenance command...");
  Serial.println();
  keepSystemResponsive();
}

void Maintenance::testSpeaker() {
  stopRequested = false;
  Serial.println("=== TESTING SPEAKER ===");
  Serial.println("Testing speaker by playing random music from cut folder...");

  if (musicFileCount == 0) {
    Serial.println("No music files found in /cut folder");
    Serial.println("Please ensure there are .mp3 or .wav files in the /cut directory on SD card");
    Serial.println("=== SPEAKER TEST FAILED ===");
    Serial.println();
    return;
  }

  // Select random music file
  int randomIndex = random(musicFileCount);
  String selectedFile = musicFiles[randomIndex];

  Serial.printf("Playing random music file (%d/%d): %s\n", randomIndex + 1, musicFileCount, selectedFile.c_str());
  Serial.println("Music should play now...");

  // Ensure audio is properly configured for playback (same as main.ino setup)
  audio->setPinout(5, 17, 18);  // I2S_BCLK, I2S_LRC, I2S_DOUT (using pin numbers from main)
  audio->setVolume(15);         // Keep volume moderate to prevent errors
  audio->setTone(0, 0, 0);      // Disable tone processing to reduce load
  audio->forceMono(true);       // Force mono to reduce processing
  audio->setBalance(0);         // Set balance to center

  // Stop any current audio and play the selected file
  audio->stopSong();
  *audioFinished = false;
  audio->connecttoFS(SD, selectedFile.c_str());

  // Wait for audio to start with more detailed checking
  unsigned long audioTimeout = millis() + 10000;  // 10 second timeout
  Serial.println("Waiting for audio to start...");
  while (!audio->isRunning() && millis() < audioTimeout) {
    if (stopRequested) return;
    audio->loop();
    keepSystemResponsive();  // Keep MQTT processing active
    delay(50);
    Serial.print(".");
  }
  Serial.println();

  if (!audio->isRunning()) {
    Serial.printf("Failed to play music file: %s\n", selectedFile.c_str());
    Serial.println("Check if:");
    Serial.println("- SD card is properly connected");
    Serial.println("- Audio file is not corrupted");
    Serial.println("- Speaker connections are correct");
    Serial.println("=== SPEAKER TEST FAILED ===");
  } else {
    Serial.println("Music started successfully!");
    Serial.println("Playing audio until finished...");
    Serial.println("Listen for audio output from the speaker");

    // Keep audio running until the file finishes - with MQTT processing
    while (audio->isRunning()) {
      if (stopRequested) return;
      audio->loop();           // Keep audio processing
      keepSystemResponsive();  // Keep MQTT processing active during playback
      delay(50);               // Small delay to prevent blocking
    }

    Serial.println("Audio playback completed");
    Serial.println("If you heard music, the speaker and SD card are working correctly");
    Serial.println("=== SPEAKER TEST COMPLETED ===");
    Serial.println("Robot ready for next maintenance command...");
  }

  Serial.println();
}

void Maintenance::testAll() {
  Serial.println("=== TESTING ALL COMPONENTS ===");
  Serial.println("Running comprehensive test of all robot components...");
  Serial.println();

  stopRequested = false;
  testHands();
  if (stopRequested) return;
  delay(2000);

  testNeck();
  if (stopRequested) return;
  delay(2000);

  testEyes();
  if (stopRequested) return;
  delay(2000);

  testSpeaker();

  Serial.println("=== ALL COMPONENTS TEST COMPLETED ===");
  Serial.println("Review all test results above");
  Serial.println("Robot ready for next maintenance command...");
  Serial.println();
}

void Maintenance::showHelp() {
  Serial.println("--- MAINTENANCE COMMANDS ---");
  Serial.println("test_hands   - Test left and right hand servos");
  Serial.println("test_neck    - Test neck servo movements");
  Serial.println("test_eyes    - Test OLED eye display");
  Serial.println("test_speaker - Test speaker by playing random music");
  Serial.println("test_all     - Run all tests sequentially");
  Serial.println("stop         - Stop current test and return to standby");
  Serial.println("help         - Show this help menu");
  Serial.println("---------------------------");
  Serial.println();
}

void Maintenance::returnToSleepState() {
  // Ensure robot returns to proper sleep state for next maintenance command
  robot->standBy();
  // Note: We don't change currentState here as it's managed by main.ino
  Serial.println("Maintenance test completed - robot ready for next command");
}

void Maintenance::keepSystemResponsive() {
  // Keep MQTT processing active during maintenance operations
  if (mqttClient && mqttClient->connected()) {
    mqttClient->loop();
  }

  // Yield control to other tasks to prevent blocking
  yield();

  // Small delay to prevent overwhelming the system
  delay(1);
}