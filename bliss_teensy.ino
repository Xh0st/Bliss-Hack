#include <Arduino.h>
#include "payload.h"

// Glitch timing from successful logs
const uint32_t G1_OFFSET = 189600;   // cycles before first glitch
const uint32_t G1_WIDTH  = 150;      // glitch pulse width (cycles)
const uint32_t G2_OFFSET = 25640;    // cycles after DAT_CLK rise before second glitch
const uint32_t G2_WIDTH  = 131;      // second glitch width

// Pins
#define GLITCH_PIN  12
#define EFUSE_PIN   A0
#define DAT_CLK_PIN 8
#define DATA_START  0

void setup() {
  pinMode(GLITCH_PIN, OUTPUT);
  pinMode(DAT_CLK_PIN, INPUT);
  for (int i = DATA_START; i < DATA_START+8; i++) pinMode(i, OUTPUT);
  digitalWrite(GLITCH_PIN, LOW);

  // Enable cycle counter
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

  Serial.begin(115200);
  Serial.println("Teensy ready. Waiting for EFUSE dip...");
}

void loop() {
  // Wait for EFUSE voltage to drop (boot start)
  while (analogRead(EFUSE_PIN) > 800);

  // --- Stage 1: Glitch to disable MPU ---
  delayCycles(G1_OFFSET);
  fireGlitch(G1_WIDTH);

  // --- Stage 2: Wait for DAT_CLK and inject payload ---
  while (digitalReadFast(DAT_CLK_PIN) == LOW);
  delayCycles(G2_OFFSET);
  digitalWriteFast(GLITCH_PIN, HIGH);   // fire glitch

  // Send payload bytes (parallel write)
  for (uint32_t i = 0; i < payload_bin_len; i++) {
    GPIO6_DR = payload_bin[i];
  }

  // Pad to 12200 bytes (original payload size)
  for (uint32_t i = payload_bin_len; i < 12200; i++) {
    GPIO6_DR = 0x00;
  }

  digitalWriteFast(GLITCH_PIN, LOW);

  // Indicate success over UART
  Serial.println("!!! BLISS_WIN: PAYLOAD INJECTED !!!");
  delay(1000);
}

void fireGlitch(uint32_t width) {
  digitalWriteFast(GLITCH_PIN, HIGH);
  delayCycles(width);
  digitalWriteFast(GLITCH_PIN, LOW);
}

inline void delayCycles(uint32_t cycles) {
  uint32_t start = ARM_DWT_CYCCNT;
  while ((ARM_DWT_CYCCNT - start) < cycles);
}
