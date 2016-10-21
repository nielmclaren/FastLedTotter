
#include <math.h>

const int quantumWidth = 32;
const int numQuanta = ceil(numLedsPerStrip / quantumWidth);
int quantum = -1;
int prevQuantum = -1;
bool quantumSwitched = false;
const int quantumFadeRate = floor((float)globalLightness / 20);

const int sparkleProbability = 3;

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
    else if (random(1000) < sparkleProbability) {
      int v = random(255);
      leds[i] = CRGB(v, v, v);
    }
    else {
      leds[i] = fadeToBlack(leds[i], quantumFadeRate);
    }
  }

  if (isSwitching) {
    Serial.println('d');
  }

  prevQuantum = quantum;
}

bool isBetweenQuanta(int q, int start, int end) {
  return start < end && start < q && q < end
    || end < q && q < start;
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
  return CHSV((millis() / 1000 * 4) % 255, globalSaturation, globalLightness);
}

