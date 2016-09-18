
#include <math.h>

const int quantumWidth = 32;
const int numQuanta = ceil(numLedsPerStrip / quantumWidth);
int quantum = -1;
int prevQuantum = -1;
bool quantumSwitched = false;
const int quantumFadeRate = 3;

void loopQuantizeMode() {
  quantum = floor((1. + tilt) / 2. * (numQuanta + 1));
  debounceQuantumFlipFlop();
  quantumSwitched = quantum != prevQuantum;

  for (int i = 0; i < numLedsPerStrip; i++) {
    int ledQuantum = floor(i / quantumWidth);
    if (quantumSwitched && (ledQuantum == quantum ||
          isBetweenQuanta(ledQuantum, prevQuantum, quantum))) {
      leds[i] = getQuantumColor();
    }
    else {
      leds[i] = fadeToBlack(leds[i], quantumFadeRate);
    }
  }

  prevQuantum = quantum;
}

bool isBetweenQuanta(int q, int start, int end) {
  return start < end && start < q && q < end
    || end < q && q < start;
}

CRGB fadeToBlack(CRGB c, int rate) {
  return CRGB(
      max(0, c.r - rate),
      max(0, c.g - rate),
      max(0, c.b - rate));
}

void debounceQuantumFlipFlop() {
  if (prevQuantum != quantum
      && abs(quantum - prevQuantum) == 1
      && getQuantumError() < 0.2) {
    quantum = prevQuantum;
  }
}

float getQuantumError() {
  float x = fmod((1 + tilt) / 2 * (numQuanta + 1), 1);
  return (float)min(x, 1 - x);
}

CRGB getQuantumColor() {
  return CHSV((millis() / 1000 * 4) % 255, 255, 64);
}

