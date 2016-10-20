
// NOTE: This file is named for alphabetical ordering.

const int maxTilt = 4;

float tilt = 0;
float tiltDirection = 0;

const int maxTiltReadings = 5;
int numTiltReadings = 0;
float tiltReadings[maxTiltReadings];

float prevTiltAverage = 0;
float tiltAverage = 0;
int tiltAverageDirection = 0;
int prevTiltAverageDirection = 0;

int tiltMoveDirection = 0;
int prevTiltMoveDirection = 0;

bool isSwitching = false;
int prevSwitchTiltDirection = 0;
unsigned long prevSwitchTime = 0;


void updateTilt() {
  tilt = constrain(CircuitPlayground.motionY() / maxTilt, -1, 1);
}

void updateTiltDirection() {
  if (tilt == 0) {
    tiltDirection = 0;
  }
  else {
    tiltDirection = abs(tilt) / tilt;
  }
}

void updateTiltReadings() {
  if (numTiltReadings < maxTiltReadings) {
    tiltReadings[numTiltReadings] = tiltReadings[numTiltReadings - 1];
    numTiltReadings++;
  }
  for (int i = numTiltReadings - 1; i > 0; i--) {
    tiltReadings[i] = tiltReadings[i - 1];
  }

  tiltReadings[0] = tilt;
  if (numTiltReadings <= 0) {
    numTiltReadings = 1;
  }
}

void updateTiltAverage() {
  prevTiltAverage = tiltAverage;
  prevTiltAverageDirection = tiltAverageDirection;
  tiltAverage = 0;
  for (int i = 0; i < numTiltReadings; i++) {
    tiltAverage += tiltReadings[i];
  }
  tiltAverage /= numTiltReadings;
  tiltAverageDirection = abs(tiltAverage) / tiltAverage;
}

void updateTiltMoveDirection() {
  float tiltMoveDirectionDelta = tiltReadings[0] - tiltReadings[numTiltReadings - 1];
  if (abs(tiltMoveDirectionDelta) > 0.05) {
    prevTiltMoveDirection = tiltMoveDirection;
    tiltMoveDirection = abs(tiltMoveDirectionDelta) / tiltMoveDirectionDelta;
  }
}

void updateSwitching() {
  isSwitching = tiltMoveDirection != prevTiltMoveDirection
      && tiltDirection != prevSwitchTiltDirection
      && abs(tilt) > 0.07;

  if (isSwitching) {
    prevSwitchTiltDirection = tiltDirection;
    prevSwitchTime = millis();
  }
}

void updateSerial() {
  if (isSwitching) {
    if (tiltDirection > 0) {
      Serial.println('a');
    }
    else {
      Serial.println('b');
    }
  }
}

CRGB fadeToBlack(CRGB c, int rate) {
  return CRGB(
      max(0, c.r - rate),
      max(0, c.g - rate),
      max(0, c.b - rate));
}

