/*
 * MEMORY SPIL - Arduino Workshop
 * Et Simon Says-lignende spil med NeoPixel LEDs og buzzer
 *
 * Hardware:
 * - 4 knapper på pin 2-5 (hvid, rød, gul, blå)
 * - 8 NeoPixel LEDs på pin 6
 * - Buzzer på pin 9
 */

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
#define MAX_NIVEAU  20
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
int sekvens[MAX_NIVEAU];  // Gemmer spillets sekvens
int niveau = 0;           // Nuværende niveau (antal trin i sekvensen)

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

  // Tilfældig seed fra ubrugt analog pin
  randomSeed(analogRead(A0));

  // Velkomst animation
  velkomst();
}

// === HOVEDLOOP ===
void loop() {
  // Tilføj nyt element til sekvensen
  sekvens[niveau] = random(0, 4);  // 0=hvid, 1=rød, 2=gul, 3=blå
  niveau++;

  delay(500);

  // Vis sekvensen
  visSekvens();

  // Spillerens tur - gentag sekvensen
  for (int i = 0; i < niveau; i++) {
    int tryk = laesKnap();  // Viser feedback automatisk

    if (tryk != sekvens[i]) {
      // Forkert! Game over
      gameOver();
      return;
    }
  }

  // Runde klaret!
  delay(500);
}

// === HJÆLPEFUNKTIONER ===

// Beregn forsinkelse baseret på niveau (hurtigere = sværere)
int getForsinkelse() {
  int forsinkelse = 600 - (niveau * 50);
  if (forsinkelse < 200) forsinkelse = 200;  // Minimum 200ms
  return forsinkelse;
}

// Vis hele sekvensen
void visSekvens() {
  int forsinkelse = getForsinkelse();

  for (int i = 0; i < niveau; i++) {
    visFarve(sekvens[i]);
    delay(forsinkelse);
    slukAlle();
    delay(150);
  }
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

  // Tænd 2 LEDs per farve (LED 0-1, 2-3, 4-5, 6-7)
  strip.setPixelColor(7-(farveNr * 2), farve);
  strip.setPixelColor(7-(farveNr * 2 + 1), farve);
  strip.show();

  // Spil tone
  tone(BUZZER, TONER[farveNr]);
}

// Sluk alle LEDs og lyd
void slukAlle() {
  strip.clear();
  strip.show();
  noTone(BUZZER);
}

// Vent på knapstryk og returner hvilken (0-3)
// Med debounce: vent, tjek igen, vent på slip
int laesKnap() {
  int knap = -1;

  // 1. Vent på at en knap trykkes
  while (knap == -1) {
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

// Game over - blink rødt og vis score
void gameOver() {
  noTone(BUZZER);

  // Blink rødt 3 gange
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < ANTAL_LEDS; j++) {
      strip.setPixelColor(j, ROED);
    }
    strip.show();
    tone(BUZZER, 150);  // Lav fejl-tone
    delay(200);
    slukAlle();
    noTone(BUZZER);
    delay(200);
  }

  // Vis score (niveau-1 fordi vi fejlede på dette niveau)
  // Tænd så mange LEDs som spilleren klarede
  int score = niveau - 1;
  for (int i = 0; i < score && i < ANTAL_LEDS; i++) {
    strip.setPixelColor(i, GUL);
  }
  strip.show();
  delay(2000);

  // Nulstil og start forfra
  slukAlle();
  niveau = 0;
  delay(1000);
}
