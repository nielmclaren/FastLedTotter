
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
  loopBeamMode();

  FastLED.show();
  delay(2);
}

