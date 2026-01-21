#include <Adafruit_NeoPixel.h>

// === PIN DEFINITIONER ===
#define KNAP_HVID   2
#define KNAP_ROED   3
#define KNAP_GUL    4
#define KNAP_BLAA   5
#define NEOPIXEL    6

// === KONSTANTER ===
#define ANTAL_LEDS  8
#define DEBOUNCE_TID 30  // Millisekunder til debounce

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
   // Initialiser knapper med intern pull-up modstand
  pinMode(KNAP_HVID, INPUT_PULLUP);
  pinMode(KNAP_ROED, INPUT_PULLUP);
  pinMode(KNAP_GUL, INPUT_PULLUP);
  pinMode(KNAP_BLAA, INPUT_PULLUP);

  // Start NeoPixel
  strip.begin();
  strip.setBrightness(150);  // Lysstyrke (0-255)
  strip.show();

  // Velkomst animation
  velkomst();
}

// === HOVEDLOOP ===
void loop() {
  if (digitalRead(KNAP_ROED) == LOW){ //Hvis knappen rød trykkes
    strip.setPixelColor(4, ROED); //tænd pixel #4
    strip.show(); //send til LED
    delay(200); //vent 0,2se
  } else { //hvis ikke trykket
    strip.clear(); //sæt alle LED til slukket
    strip.show();  //send det til LED
  }
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

