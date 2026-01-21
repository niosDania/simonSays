/*
 * =====================================================
 * TRIN 6: KOMPLET SPIL
 * =====================================================
 *
 * NYT i dette trin:
 *   - Unikke toner for hver farve
 *   - Hastigheden øges med niveauet
 *   - Ordentlig debounce på knapper
 *   - Feedback mens knappen holdes nede
 *
 * Dette er det færdige Memory Spil!
 *
 * Hardware: NeoPixel + 4 knapper + buzzer
 * =====================================================
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
#define ANTAL_LEDS    8
#define MAX_NIVEAU    20
#define DEBOUNCE_TID  30   // ===== NYT: Debounce tid i ms =====

// === NYT: TONER FOR HVER FARVE ===
// Musikalske toner (Hz): C, E, G, høj C
const int TONER[] = {262, 330, 392, 523};

Adafruit_NeoPixel strip(ANTAL_LEDS, NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Spillets hukommelse
int sekvens[MAX_NIVEAU];
int niveau = 0;

void setup() {
  pinMode(KNAP_HVID, INPUT_PULLUP);
  pinMode(KNAP_ROED, INPUT_PULLUP);
  pinMode(KNAP_GUL, INPUT_PULLUP);
  pinMode(KNAP_BLAA, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  strip.begin();
  strip.setBrightness(150);
  strip.show();

  randomSeed(analogRead(A0));
  velkomst();
}

void loop() {
  // Tilføj ny farve
  sekvens[niveau] = random(0, 4);
  niveau++;

  delay(500);

  // Vis sekvensen
  visSekvens();

  // Spillerens tur
  for (int i = 0; i < niveau; i++) {
    int svar = laesKnap();  // Viser feedback automatisk

    if (svar != sekvens[i]) {
      gameOver();
      return;
    }
  }

  // Runde klaret!
  delay(300);
}

// =====================================================
// ===== NYT: HASTIGHED DER ØGES =====
// =====================================================

/*
 * getForsinkelse() - Beregn visningstid baseret på niveau
 *
 * Start: 600ms, reduceres med 50ms per niveau
 * Minimum: 200ms (ellers bliver det umuligt!)
 */
int getForsinkelse() {
  int forsinkelse = 600 - (niveau * 50);
  if (forsinkelse < 200) {
    forsinkelse = 200;
  }
  return forsinkelse;
}

// =====================================================
// ===== NYT: VIS FARVE MED LYD =====
// =====================================================

/*
 * visFarve() - Tænd LEDs OG spil tone
 */
void visFarve(int farveNr) {
  slukAlle();

  // Bestem farve
  int r, g, b;
  switch (farveNr) {
    case 0: r = 255; g = 255; b = 255; break;  // Hvid
    case 1: r = 255; g = 0;   b = 0;   break;  // Rød
    case 2: r = 255; g = 255; b = 0;   break;  // Gul
    case 3: r = 0;   g = 0;   b = 255; break;  // Blå
  }

  // Tænd LEDs
  int led = 7 - (farveNr * 2);
  strip.setPixelColor(led, r, g, b);
  strip.setPixelColor(led - 1, r, g, b);
  strip.show();

  // ===== NYT: Spil tone =====
  tone(BUZZER, TONER[farveNr]);
}

void slukAlle() {
  strip.clear();
  strip.show();
  noTone(BUZZER);
}

/*
 * visSekvens() - Vis sekvensen med dynamisk hastighed
 */
void visSekvens() {
  int forsinkelse = getForsinkelse();  // ===== NYT: Bruger niveau =====

  for (int i = 0; i < niveau; i++) {
    visFarve(sekvens[i]);
    delay(forsinkelse);
    slukAlle();
    delay(150);
  }
}

// =====================================================
// ===== NYT: DEBOUNCE PÅ KNAPPER =====
// =====================================================

/*
 * laesKnap() - Læs knapstryk med ordentlig debounce
 *
 * Debounce-problemet:
 *   Når en knap trykkes, "hopper" kontakten og sender
 *   mange tænd/sluk signaler på få millisekunder.
 *
 * Løsning:
 *   1. Vent på at knap trykkes
 *   2. Vent DEBOUNCE_TID ms
 *   3. Vis feedback mens knappen holdes
 *   4. Vent på at knappen slippes
 *   5. Vent DEBOUNCE_TID ms igen
 */
int laesKnap() {
  int knap = -1;

  // 1. Vent på knapstryk
  while (knap == -1) {
    if (digitalRead(KNAP_HVID) == LOW) knap = 0;
    if (digitalRead(KNAP_ROED) == LOW) knap = 1;
    if (digitalRead(KNAP_GUL) == LOW)  knap = 2;
    if (digitalRead(KNAP_BLAA) == LOW) knap = 3;
  }

  // 2. Debounce ved tryk
  delay(DEBOUNCE_TID);

  // 3. Vis feedback (LED + lyd)
  visFarve(knap);

  // 4. Vent på at knappen slippes
  int pins[] = {KNAP_HVID, KNAP_ROED, KNAP_GUL, KNAP_BLAA};
  while (digitalRead(pins[knap]) == LOW) {
    // Holder feedback tændt...
  }

  // 5. Debounce ved slip
  delay(DEBOUNCE_TID);

  slukAlle();
  return knap;
}

// =====================================================
// ØVRIGE FUNKTIONER
// =====================================================

void velkomst() {
  for (int i = 0; i < ANTAL_LEDS; i++) {
    strip.setPixelColor(i, 0, 0, 255);
    strip.show();
    delay(80);
  }
  delay(300);
  slukAlle();
  delay(500);
}

void gameOver() {
  noTone(BUZZER);

  // Blink rødt 3 gange
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < ANTAL_LEDS; j++) {
      strip.setPixelColor(j, 255, 0, 0);
    }
    strip.show();
    tone(BUZZER, 150);
    delay(200);
    slukAlle();
    noTone(BUZZER);
    delay(200);
  }

  // Vis score
  int score = niveau - 1;
  for (int i = 0; i < score && i < ANTAL_LEDS; i++) {
    strip.setPixelColor(i, 255, 255, 0);
  }
  strip.show();
  delay(2000);

  // Nulstil
  slukAlle();
  niveau = 0;
  delay(1000);
}

/*
 * =====================================================
 * TILLYKKE! DU HAR LAVET ET KOMPLET SPIL!
 * =====================================================
 *
 * Hvad du har lært:
 *   - NeoPixel LED-styring
 *   - Knap-input med INPUT_PULLUP
 *   - Funktioner til at organisere kode
 *   - Arrays til at gemme data
 *   - for-loops til gentagelse
 *   - random() til tilfældighed
 *   - tone() til lyd
 *   - Debounce til pålidelig input
 *
 * =====================================================
 * UDFORDRINGER:
 * =====================================================
 *
 * 1. Tilføj en "highscore" der vises ved game over
 *
 * 2. Lav en svær mode der starter hurtigere
 *
 * 3. Tilføj en 5. knap der starter spillet forfra
 *
 * 4. Gem highscore i EEPROM så den huskes
 *    (hint: #include <EEPROM.h>)
 *
 * =====================================================
 */
