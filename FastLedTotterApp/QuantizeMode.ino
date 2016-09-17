
#include <math.h>

const int quantumWidth = 16;
const int numQuanta = ceil(numLedsPerStrip / quantumWidth);
int quantum;
int prevQuantum;
int prevPrevQuantum;
unsigned long quantumSwitchTime;

void setupQuantizeMode() {
  quantum = -1;
  prevQuantum = -1;
  prevPrevQuantum = -1;
  quantumSwitchTime = 0;
}

void loopQuantizeMode() {
  quantum = floor((1. + tilt) / 2. * numQuanta);
  debounceQuantumFlipFlop();

  for (int i = 0; i < numLedsPerStrip; i++) {
    if (floor(i / quantumWidth) == quantum) {
      leds[i] = getQuantumColor(millis() - quantumSwitchTime);
    }
    else {
      leds[i] = CRGB::Black;
    }
  }
}

void debounceQuantumFlipFlop() {
  if (prevQuantum != quantum) {
    if (prevPrevQuantum == quantum && getQuantumError() < 0.2) {
      quantum = prevQuantum;
    }
    else {
      quantumSwitchTime = millis();
      prevPrevQuantum = prevQuantum;
      prevQuantum = quantum;
    }
  }
}

float getQuantumError() {
  float x = fmod((1 + tilt) / 2 * numQuanta, 1);
  return (float)min(x, 1 - x);
}

CRGB getQuantumColor(unsigned long age) {
  int x;
  if (age < 200) {
    x = map(age, 0, 200, 64, 0);
  }
  else {
    x = 0;
  }
  return CHSV((millis() / 1000 * 4) % 255, 255, x);
}

