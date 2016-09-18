
void loopLevelMode() {
  float t = modTime(1500);
  CRGB color;
  int halfWidth = 3;
  int tiltLedIndex = floor(numLedsPerStrip * (1 + tiltAverage) / 2);
  float deltaLedIndex = mapf(tiltReadings[0] - tiltReadings[numTiltReadings - 1], -2, 2, 0, numLedsPerStrip);
  float wavelengthFactor = 0.8;

  for (int i = 0; i < numLedsPerStrip; i++) {
    if (tiltLedIndex == i || deltaLedIndex == i) {
      color = 0x030303;
    }
    else if (tiltLedIndex - halfWidth < i && i < tiltLedIndex + halfWidth
        || deltaLedIndex - halfWidth < i && i < deltaLedIndex + halfWidth) {
      color = CRGB::Black;
    }
    else {
      float u = splitTime(clampTime(t - wavelengthFactor * ((float)abs(i - tiltLedIndex) / numLedsPerStrip)));
      color = CHSV(mapf(u, 0, 1, 0, 30), globalSaturation, random(globalLightness - 1, globalLightness + 1));
    }

    leds[i] = color;
  }
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

