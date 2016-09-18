
#include <Adafruit_CircuitPlayground.h>
#include <FastLED.h>

const int numLedsPerStrip = 300;
CRGB leds[numLedsPerStrip];

const int globalSaturation = 255;
const int globalLightness = 64;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  CircuitPlayground.begin();

  FastLED.addLeds<NEOPIXEL, 6>(leds, numLedsPerStrip);
  FastLED.addLeds<NEOPIXEL, 9>(leds, numLedsPerStrip);
}

void loop() {
  updateTilt();
  updateTiltDirection();
  updateTiltReadings();
  updateTiltAverage();
  updateTiltMoveDirection();
  updateSwitching();

  //loopQuantizeMode();
  //loopBeamMode();
  loopLevelMode();

  FastLED.show();
  delay(2);
}

float modTime(long period) {
  return (float)(millis() % period) / period;
}

float clampTime(float t) {
  while (t >= 1) {
    t -= 1;
  }
  while (t < 0) {
    t += 1;
  }
  return t;
}

float splitTime(float t) {
  if (t < 0.5) {
    return 2.0 * t;
  }
  else {
    return (1.0 - t) * 2.0;
  }
}
