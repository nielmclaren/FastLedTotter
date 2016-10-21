
#include <math.h>

const int maxBeams = 8;
int numForwardBeams = 0;
int numBackwardBeams = 0;
int forwardBeamPositions[maxBeams];
int backwardBeamPositions[maxBeams];
const int beamWidth = 16;
const int flashWidth = 4;
const int beamSpeed = 12;
int prevBeamCreatedDirection = 0;
const CRGB combinedBeamColor = 0x663366;
const CRGB forwardBeamColor = CHSV(120, globalSaturation, globalLightness);
const CRGB forwardFiringColor = 0x999999;
const CRGB backwardBeamColor = CHSV(240, globalSaturation, globalLightness);
const CRGB backwardFiringColor = 0x999999;
boolean poweredUp = false;

void loopBeamMode() {
  stepBeams();
  createBeams();

  if (abs(tilt) < 0.15) {
    // Allow beam alternation to reset if we make it to halfway.
    prevBeamCreatedDirection = 0;
  }

  for (int i = 0; i < numLedsPerStrip; i++) {
     leds[i] = getBeamModeColor(i);
  }

  if (millis() - prevModeChangeTime > 30000) {
    if (!poweredUp) {
      Serial.println('b');
      poweredUp = true;
    }
  }
  else {
    poweredUp = false;
  }

  if (isSwitching) {
    if (poweredUp) {
      Serial.println('c');
    }
    else {
      Serial.println('a');
    }
  }
}

void stepBeams() {
  for (int i = numForwardBeams - 1; i >= 0; i--) {
    forwardBeamPositions[i] += beamSpeed;
    if (forwardBeamPositions[i] > numLedsPerStrip) {
      numForwardBeams--;
    }
  }

  for (int i = numBackwardBeams - 1; i >= 0; i--) {
    backwardBeamPositions[i] -= beamSpeed;
    if (backwardBeamPositions[i] < -beamWidth) {
      numBackwardBeams--;
    }
  }
}

void createBeams() {
  if (isSwitching) {
    if (tiltAverageDirection > 0) {
      createForwardBeam();
    }
    else {
      createBackwardBeam();
    }

    prevBeamCreatedDirection = tiltAverageDirection;
  }
}

void createForwardBeam() {
  if (numForwardBeams < maxBeams) {
    forwardBeamPositions[numForwardBeams] = forwardBeamPositions[numForwardBeams - 1];
    numForwardBeams++;
  }
  for (int i = numForwardBeams - 1; i > 0; i--) {
    forwardBeamPositions[i] = forwardBeamPositions[i - 1];
  }

  forwardBeamPositions[0] = -beamWidth;
  if (numForwardBeams <= 0) {
    numForwardBeams = 1;
  }
}

void createBackwardBeam() {
  if (numBackwardBeams < maxBeams) {
    backwardBeamPositions[numBackwardBeams] = backwardBeamPositions[numBackwardBeams - 1];
    numBackwardBeams++;
  }
  for (int i = numBackwardBeams - 1; i > 0; i--) {
    backwardBeamPositions[i] = backwardBeamPositions[i - 1];
  }

  backwardBeamPositions[0] = numLedsPerStrip - 1;
  if (numBackwardBeams <= 0) {
    numBackwardBeams = 1;
  }
}

CRGB getBeamModeColor(int led) {
  bool hasForwardBeam = false;
  bool hasBackwardBeam = false;
  bool isFiring = false;

  for (int i = 0; i < numForwardBeams; i++) {
    if (forwardBeamPositions[i] <= led && led < forwardBeamPositions[i] + beamWidth) {
      hasForwardBeam = true;
      if (led < flashWidth) {
        isFiring = true;
      }
      break;
    }
  }
  for (int i = 0; i < numBackwardBeams; i++) {
    if (backwardBeamPositions[i] <= led && led < backwardBeamPositions[i] + beamWidth) {
      hasBackwardBeam = true;
      if (led > numLedsPerStrip - flashWidth - 1) {
        isFiring = true;
      }
      break;
    }
  }

  if (hasForwardBeam && hasBackwardBeam) {
    return combinedBeamColor;
  }
  if (hasForwardBeam) {
    if (isFiring) {
      return forwardFiringColor;
    }
    return forwardBeamColor;
  }
  if (hasBackwardBeam) {
    if (isFiring) {
      return backwardFiringColor;
    }
    return backwardBeamColor;
  }

  return getBeamBackgroundColor(led);
}

CRGB getBeamBackgroundColor(int led) {
  float t = modTime(3000);
  t -= 4.5 * abs(led - numLedsPerStrip/2) / numLedsPerStrip;
  t = splitTime(clampTime(t));
  return CHSV(180, globalSaturation, mapf(t, 0, 1, 32, globalLightness / 3));
}
