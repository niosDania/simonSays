#include <Adafruit_NeoPixel.h>

// === PIN DEFINITIONER ===
#define KNAP_HVID   2
#define KNAP_ROED   3
#define KNAP_GUL    4
#define KNAP_BLAA   5
#define NEOPIXEL    6
#define BUZZER      9

// === KONSTANTER ===
#define ANTAL_LEDS  8
#define DEBOUNCE_TID 30  // Millisekunder til debounce

// === FARVER (RGB) ===
const uint32_t HVID  = 0xFFFFFF;
const uint32_t ROED  = 0xFF0000;
const uint32_t GUL   = 0xFFFF00;
const uint32_t BLAA  = 0x0000FF;
const uint32_t SLUKKET = 0x000000;

// Toner for hver farve (Hz)
const int TONER[] = {262, 330, 392, 523};  // C, E, G, C (højere)


// === GLOBALE VARIABLER ===
Adafruit_NeoPixel strip(ANTAL_LEDS, NEOPIXEL, NEO_GRB + NEO_KHZ800);

// === SETUP ===
void setup() {
   // Initialiser knapper med intern pull-up modstand
  pinMode(KNAP_HVID, INPUT_PULLUP);
  pinMode(KNAP_ROED, INPUT_PULLUP);
  pinMode(KNAP_GUL, INPUT_PULLUP);
  pinMode(KNAP_BLAA, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  // Start NeoPixel
  strip.begin();
  strip.setBrightness(150);  // Lysstyrke (0-255)
  strip.show();

  tone(BUZZER, TONER[2]); //start lyd
  delay(1000);
  noTone(BUZZER); //sluk lyd


  // Velkomst animation
  velkomst(); 
}

// === HOVEDLOOP ===
void loop() {
  int tryk = laesKnap();
  visFarve(tryk);
  slukAlle();  //sluk alle LED
  noTone(BUZZER); //sluk lyd

}


// === HJÆLPEFUNKTIONER ===

// Vent på knapstryk og returner hvilken (0-3)
// Med debounce: vent, tjek igen, vent på slip
int laesKnap() {
  int knap = -1; //vi sætter valgt knap til -1 (altså ingen knap valgt)

  // 1. Vent på at en knap trykkes
  while (knap == -1) { //så længe der stadig ikke er valgt knap tjekker vi om nogen af dem er trykket
    if (digitalRead(KNAP_HVID) == LOW) knap = 0;
    if (digitalRead(KNAP_ROED) == LOW) knap = 1;
    if (digitalRead(KNAP_GUL) == LOW)  knap = 2;
    if (digitalRead(KNAP_BLAA) == LOW) knap = 3;
  }

  // 2. Debounce: vent lidt og tjek at knappen stadig er trykket
  delay(DEBOUNCE_TID);

  // 3. Vis feedback mens knappen holdes nede
  visFarve(knap);

  // 4. Vent på at knappen slippes
  int pins[] = {KNAP_HVID, KNAP_ROED, KNAP_GUL, KNAP_BLAA};
  while (digitalRead(pins[knap]) == LOW) {
    // Venter...
  }

  // 5. Debounce ved slip
  delay(DEBOUNCE_TID);

  slukAlle();
  return knap;
}

// Vis én farve på LEDs og spil tone
void visFarve(int farveNr) {
  uint32_t farve;

  switch (farveNr) {
    case 0: farve = HVID; break;
    case 1: farve = ROED; break;
    case 2: farve = GUL;  break;
    case 3: farve = BLAA; break;
  }

  // Tænd 2 LEDs per farve (fra højre: 7-6, 5-4, 3-2, 1-0)
  int led = 7 - (farveNr * 2);
  strip.setPixelColor(led, farve);
  strip.setPixelColor(led - 1, farve);
  strip.show();

  // Spil tone
  tone(BUZZER, TONER[farveNr]);
}

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

