
#include <Adafruit_CircuitPlayground.h>
#include <FastLED.h>

const int numLedsPerStrip = 300;
CRGB leds[numLedsPerStrip];

const int globalSaturation = 255;
const int globalLightness = 64;

const int maxTilt = 4;
float tilt;
float tiltDirection;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  CircuitPlayground.begin();

  FastLED.addLeds<NEOPIXEL, 6>(leds, numLedsPerStrip);
  FastLED.addLeds<NEOPIXEL, 9>(leds, numLedsPerStrip);

  tilt = 0;

  setupQuantizeMode();
}

void loop() {
  readTilt();
  readTiltDirection();

  loopQuantizeMode();

  FastLED.show();
  delay(2);
}

void readTilt() {
  tilt = constrain(CircuitPlayground.motionX() / maxTilt, -1, 1);
}

void readTiltDirection() {
  if (tilt == 0) {
    tiltDirection = 0;
  }
  else {
    tiltDirection = abs(tilt) / tilt;
  }
}
