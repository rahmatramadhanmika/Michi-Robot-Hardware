#include "neck.h"
#include "eyes.h"  // Include eyes header

Neck::Neck(int servoPin)
  : servoPin(servoPin) {
}

void Neck::begin() {
  servo1.attach(servoPin);
  servo1.write(80);
  randomSeed(analogRead(0));
}

void Neck::setEyesReference(eyes* eyesPtr) {
  this->eyesPtr = eyesPtr;
}

void Neck::idle() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateMillis < interval) {
    return;
  }
  lastUpdateMillis = currentMillis;

  if (!waitingAtTarget && !returningToCenter && currentMillis - lastMoveMillis >= random(3000, 70001)) {
    targetDegrees = random(30, 151);
    lastMoveMillis = currentMillis;
  }

  if (posDegrees != targetDegrees && !waitingAtTarget && !returningToCenter) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      waitingAtTarget = true;
      waitStartMillis = currentMillis;
      
      // Trigger eye movement based on neck position during idle
      if (eyesPtr != nullptr) {
        if (posDegrees >= 110) {
          eyesPtr->startMoveBigEye(1);  // Look right when neck turns right
        } else if (posDegrees <= 50) {
          eyesPtr->startMoveBigEye(-1); // Look left when neck turns left
        }
      }
    }
  } else if (waitingAtTarget && currentMillis - waitStartMillis >= random(2000, 3001)) {
    waitingAtTarget = false;
    returningToCenter = true;
    targetDegrees = 80;
  } else if (returningToCenter && posDegrees != targetDegrees) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      returningToCenter = false;
      lastMoveMillis = currentMillis;
    }
  }
}

void Neck::answer() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateMillis < answerInterval) {
    return;
  }
  lastUpdateMillis = currentMillis;

  if (!waitingAtTarget && !returningToCenter && currentMillis - lastMoveMillis >= random(7000, 10001)) {
    targetDegrees = random(60, 121);
    lastMoveMillis = currentMillis;
  }

  if (posDegrees != targetDegrees && !waitingAtTarget && !returningToCenter) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      waitingAtTarget = true;
      waitStartMillis = currentMillis;
    }
  } else if (waitingAtTarget && currentMillis - waitStartMillis >= random(1000, 2001)) {
    waitingAtTarget = false;
    returningToCenter = true;
    targetDegrees = 80;
  } else if (returningToCenter && posDegrees != targetDegrees) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      returningToCenter = false;
      lastMoveMillis = currentMillis;
    }
  }
}

void Neck::sad() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateMillis < sadInterval) {
    return;
  }
  lastUpdateMillis = currentMillis;

  if (!waitingAtTarget && !returningToCenter && currentMillis - lastMoveMillis >= random(2000, 3001)) {
    targetDegrees = movingTo110 ? 110 : 80;
    movingTo110 = !movingTo110;
    lastMoveMillis = currentMillis;
  }

  if (posDegrees != targetDegrees && !waitingAtTarget && !returningToCenter) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      waitingAtTarget = true;
      waitStartMillis = currentMillis;
    }
  } else if (waitingAtTarget && currentMillis - waitStartMillis >= random(1000, 2001)) {
    waitingAtTarget = false;
    returningToCenter = true;
    targetDegrees = 80;
  } else if (returningToCenter && posDegrees != targetDegrees) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      returningToCenter = false;
      lastMoveMillis = currentMillis;
    }
  }
}

void Neck::happy() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateMillis < answerInterval) {
    return;
  }
  lastUpdateMillis = currentMillis;

  if (!waitingAtTarget && !returningToCenter && currentMillis - lastMoveMillis >= random(2000, 3001)) {
    targetDegrees = random(60, 121);
    lastMoveMillis = currentMillis;
  }

  if (posDegrees != targetDegrees && !waitingAtTarget && !returningToCenter) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      waitingAtTarget = true;
      waitStartMillis = currentMillis;
    }
  } else if (waitingAtTarget && currentMillis - waitStartMillis >= random(1000, 2001)) {
    waitingAtTarget = false;
    returningToCenter = true;
    targetDegrees = 80;
  } else if (returningToCenter && posDegrees != targetDegrees) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      returningToCenter = false;
      lastMoveMillis = currentMillis;
    }
  }
}

void Neck::standBy() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateMillis < interval) {
    return;
  }
  lastUpdateMillis = currentMillis;

  if (posDegrees != 80) {
    if (posDegrees > 80) {
      posDegrees--;
    } else if (posDegrees < 80) {
      posDegrees++;
    }
    servo1.write(posDegrees);
  }
}

void Neck::detectWakeword() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateMillis < interval) {
    return;
  }
  lastUpdateMillis = currentMillis;

  // Start new cycle after random delay (2-3 seconds) - randomly choose 120° or 40°
  if (!waitingAtTarget && !returningToCenter && currentMillis - lastMoveMillis >= random(2000, 3001)) {
    // Randomly choose between 120° (right) or 40° (left)
    targetDegrees = random(0, 2) == 0 ? 120 : 40;
    lastMoveMillis = currentMillis;
    Serial.printf("DetectWakeword: Starting cycle - moving to %d°\n", targetDegrees);
  }

  // Move towards target
  if (posDegrees != targetDegrees && !waitingAtTarget && !returningToCenter) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      waitingAtTarget = true;
      waitStartMillis = currentMillis;
      Serial.printf("DetectWakeword: Reached target %d°, waiting...\n", targetDegrees);
    }
  }
  // Wait at target (120° or 40°) for 1-2 seconds, then return to center
  else if (waitingAtTarget && currentMillis - waitStartMillis >= random(1000, 2001)) {
    waitingAtTarget = false;
    returningToCenter = true;
    targetDegrees = 80; // Always return to center (80°)
    Serial.println("DetectWakeword: Returning to center 80°");
    lastMoveMillis = currentMillis;
  }
  // Return to center (80 degrees)
  else if (returningToCenter && posDegrees != targetDegrees) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      returningToCenter = false;
      lastMoveMillis = currentMillis; // Reset timer for random delay
      Serial.println("DetectWakeword: Returned to center 80°, cycle complete");
    }
  }
}

void Neck::thinking() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateMillis < interval) {
    return;
  }
  lastUpdateMillis = currentMillis;

  // Start new thinking cycle after random delay (3-5 seconds)
  if (!waitingAtTarget && !returningToCenter && currentMillis - lastMoveMillis >= random(1000, 3001)) {
    targetDegrees = random(30, 151); // Select new random target between 30 and 150 degrees
    lastMoveMillis = currentMillis;
    Serial.printf("Thinking: Starting new cycle - moving to %d°\n", targetDegrees);
  }

  // Move towards target
  if (posDegrees != targetDegrees && !waitingAtTarget && !returningToCenter) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      waitingAtTarget = true;
      waitStartMillis = currentMillis;
      Serial.printf("Thinking: Reached target %d°, waiting...\n", targetDegrees);
    }
  } 
  // Wait at target for 2-3 seconds, then return to center
  else if (waitingAtTarget && currentMillis - waitStartMillis >= random(2000, 3001)) {
    waitingAtTarget = false;
    returningToCenter = true;
    targetDegrees = 80; // Return to center position
    Serial.println("Thinking: Returning to center 80°");
  } 
  // Return to center position
  else if (returningToCenter && posDegrees != targetDegrees) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      returningToCenter = false;
      lastMoveMillis = currentMillis; // Reset timer for next thinking cycle
      Serial.println("Thinking: Returned to center, thinking cycle complete");
    }
  }
}

void Neck::resetState() {
  waitingAtTarget = false;
  returningToCenter = false;
  targetDegrees = 80;
  lastMoveMillis = millis();
  lastUpdateMillis = millis();
  Serial.println("Neck: State reset completed");
}

void Neck::testNeck() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateMillis < interval) {
    return;
  }
  lastUpdateMillis = currentMillis;

  // If at target and waiting, check if wait time (3-5 seconds) has elapsed
  if (waitingAtTarget && currentMillis - waitStartMillis >= random(3000, 5001)) {
    waitingAtTarget = false;
    targetDegrees = random(0, 181); // Select new random target between 30 and 150 degrees
    lastMoveMillis = currentMillis;
  }

  // If not waiting and at target, start waiting
  if (!waitingAtTarget && posDegrees == targetDegrees) {
    waitingAtTarget = true;
    waitStartMillis = currentMillis;
  }

  // Move towards target
  if (posDegrees != targetDegrees && !waitingAtTarget) {
    if (posDegrees < targetDegrees) {
      posDegrees++;
    } else {
      posDegrees--;
    }
    servo1.write(posDegrees);
    if (posDegrees == targetDegrees) {
      waitingAtTarget = true;
      waitStartMillis = currentMillis;
      lastMoveMillis = currentMillis; // Reset timer for next delay
    }
  }
}