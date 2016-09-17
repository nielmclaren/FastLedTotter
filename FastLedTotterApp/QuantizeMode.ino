
#include <math.h>

const int quantumWidth = 16;
const int numQuanta = ceil(numLedsPerStrip / quantumWidth);
int quantum;
int prevQuantum;
int prevPrevQuantum;
unsigned long quantumSwitchTime;
bool quantumSwitched;
const int quantumFadeRate = 2;

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
    if (quantumSwitched && floor(i / quantumWidth) == quantum) {
      leds[i] = getQuantumColor();
    }
    else {
      leds[i] = fadeToBlack(leds[i], quantumFadeRate);
    }
  }
}

CRGB fadeToBlack(CRGB c, int rate) {
  return CRGB(
      max(0, c.r - rate),
      max(0, c.g - rate),
      max(0, c.b - rate));
}

void debounceQuantumFlipFlop() {
  if (prevQuantum != quantum) {
    if (prevPrevQuantum == quantum && getQuantumError() < 0.2) {
      quantum = prevQuantum;
      quantumSwitched = false;
    }
    else {
      quantumSwitchTime = millis();
      quantumSwitched = true;
      prevPrevQuantum = prevQuantum;
      prevQuantum = quantum;
    }
  }
  else {
    quantumSwitched = false;
  }
}

float getQuantumError() {
  float x = fmod((1 + tilt) / 2 * numQuanta, 1);
  return (float)min(x, 1 - x);
}

CRGB getQuantumColor() {
  return CHSV((millis() / 1000 * 4) % 255, 255, 64);
}

