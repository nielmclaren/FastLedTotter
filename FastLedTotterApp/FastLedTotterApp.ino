
#include <Adafruit_CircuitPlayground.h>
#include <FastLED.h>

const int numLedsPerStrip = 300;
CRGB leds[numLedsPerStrip];

const int globalSaturation = 255;
const int globalLightness = 64;

bool leftButtonDebouncer = false;
bool rightButtonDebouncer = false;

int mode = 0;

#define MODE_TEST -1
#define MODE_IDLE 0
#define MODE_QUANTIZE 1
#define MODE_BEAM 2
#define MODE_LEVEL 3

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

  updateButtonHandlers();

  loopMode();

  FastLED.show();
  delay(2);
}

void updateButtonHandlers() {
  if (CircuitPlayground.leftButton()) {
    if (!leftButtonDebouncer) {
      leftButtonDebouncer = true;
      leftButtonHandler();
    }
  }
  else {
    leftButtonDebouncer = false;
  }

  if (CircuitPlayground.rightButton()) {
    if (!rightButtonDebouncer) {
      rightButtonDebouncer = true;
      rightButtonHandler();
    }
  }
  else {
    rightButtonDebouncer = false;
  }
}

void leftButtonHandler() {
  if (--mode < 1) {
    mode = 3;
  }
}

void rightButtonHandler() {
  if (++mode > 3) {
    mode = 1;
  }
}

void loopMode() {
  switch (mode) {
    case MODE_TEST:
      break;
    case MODE_IDLE:
      break;
    case MODE_QUANTIZE:
      loopQuantizeMode();
      break;
    case MODE_BEAM:
      loopBeamMode();
      break;
    case MODE_LEVEL:
      loopLevelMode();
      break;
  }
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
