

#include <Adafruit_NeoPixel.h>

// === PIN DEFINITIONER ===
#define NEOPIXEL    6

// === KONSTANTER ===
#define ANTAL_LEDS  8
#define MAX_NIVEAU  20

// === FARVER (RGB) ===
const uint32_t HVID  = 0xFFFFFF;
const uint32_t ROED  = 0xFF0000;
const uint32_t GUL   = 0xFFFF00;
const uint32_t BLAA  = 0x0000FF;
const uint32_t SLUKKET = 0x000000;

// === GLOBALE VARIABLER ===
Adafruit_NeoPixel strip(ANTAL_LEDS, NEOPIXEL, NEO_GRB + NEO_KHZ800);

// === SETUP ===
void setup() {
  // Start NeoPixel
  strip.begin();
  strip.setBrightness(150);  // Lysstyrke (0-255)
  strip.show();

  // Velkomst animation
  velkomst();
}

// === HOVEDLOOP ===
void loop() {
 
}


// === HJÆLPEFUNKTIONER ===

// Sluk alle LEDs og lyd
void slukAlle() {
  strip.clear();
  strip.show();
}


// Velkomst animation
void velkomst() {
  for (int i = 0; i < ANTAL_LEDS; i++) {
    strip.setPixelColor(i, BLAA);
    strip.show();
    delay(100);
  }
  delay(500);
  slukAlle();
  delay(500);
}

