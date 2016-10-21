
#include <Adafruit_CircuitPlayground.h>
#include <FastLED.h>

const int numLedsPerStrip = 221;
CRGB leds[numLedsPerStrip];

const int globalSaturation = 192;
const int globalLightness = 255;

bool leftButtonDebouncer = false;
bool rightButtonDebouncer = false;

const int numModes = 2;
const int MODE_QUANTIZE = 0;
const int MODE_BEAM = 1;
const unsigned long modeDuration = 273000;

int mode = MODE_QUANTIZE;
int prevMode = -1;
unsigned long prevModeChangeTime;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  CircuitPlayground.begin();

  FastLED.addLeds<NEOPIXEL, 6>(leds, numLedsPerStrip);
  FastLED.addLeds<NEOPIXEL, 9>(leds, numLedsPerStrip);

  prevModeChangeTime = millis();
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
  if (--mode < 0) {
    mode = numModes - 1;
  }
}

void rightButtonHandler() {
  if (++mode >= numModes) {
    mode = 0;
  }
}

void loopMode() {
  mode = (millis() / modeDuration) % numModes;
  if (mode != prevMode) {
    prevModeChangeTime = millis();
    prevMode = mode;
  }

  switch (mode) {
    case MODE_QUANTIZE:
      loopQuantizeMode();
      break;
    case MODE_BEAM:
      loopBeamMode();
      break;
  }

  for (int i = 0; i < 10; i++) {
    if (mode == i) {
      CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(25 * i));
    }
    else {
      CircuitPlayground.setPixelColor(i, 0x000000);
    }
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
