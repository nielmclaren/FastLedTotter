
#include <math.h>

const int quantumWidth = 32;
const int numQuanta = ceil(numLedsPerStrip / quantumWidth);
int quantum;
int prevQuantum;
bool quantumSwitched;
const int quantumFadeRate = 3;

void setupQuantizeMode() {
  quantum = -1;
  prevQuantum = -1;
}

void loopQuantizeMode() {
  quantum = floor((1. + tilt) / 2. * numQuanta);
  debounceQuantumFlipFlop();
  quantumSwitched = quantum != prevQuantum;

  for (int i = 0; i < numLedsPerStrip; i++) {
    int ledQuantum = floor(i / quantumWidth);
    if (quantumSwitched && isBetweenQuanta(ledQuantum, prevQuantum, quantum)) {
      leds[i] = getQuantumColor();
    }
    else {
      leds[i] = fadeToBlack(leds[i], quantumFadeRate);
    }
  }

  prevQuantum = quantum;
}

bool isBetweenQuanta(int q, int start, int end) {
  return start < end && start < q && q <= end
    || end <= q && q < start;
}

CRGB fadeToBlack(CRGB c, int rate) {
  return CRGB(
      max(0, c.r - rate),
      max(0, c.g - rate),
      max(0, c.b - rate));
}

void debounceQuantumFlipFlop() {
  if (prevQuantum != quantum) {
    if (getQuantumError() < 0.2) {
      quantum = prevQuantum;
    }
  }
}

float getQuantumError() {
  float x = fmod((1 + tilt) / 2 * numQuanta, 1);
  return (float)min(x, 1 - x);
}

CRGB getQuantumColor() {
  return CHSV((millis() / 1000 * 4) % 255, 255, 64);
}

