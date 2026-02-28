// For Randles, Merry Christmas <3
// By Brentyn Hananana

#include <FastLED.h>

#define LED_PIN 3
#define NUM_LEDS 10
#define BRIGHTNESS 64
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define TOUCH_PIN 12
#define BUZZ_PIN 11

int counterState;
int patternState;
bool touchState;

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 24

CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p FL_PROGMEM;

void ChangePalettePeriodically();
void FillLEDsFromPaletteColors(uint8_t colorIndex);
void SetupYellowAndGreenPalette();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();

void setup() {
  pinMode(LED_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  delay(3000);  // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  SetupYellowAndGreenPalette();
  currentBlending = LINEARBLEND;
  counterState = 1;
  touchState = 0;
  patternState = 1;
}


void loop() {
  touchState = digitalRead(TOUCH_PIN);

  if (touchState == HIGH) {
    counterState++;
    tone(BUZZ_PIN, 670, 69);
    delay(300);
  }

  ChangePalettePeriodically();

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors(startIndex);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t brightness = 255;

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically() {

  if (patternState != counterState) {

    patternState = counterState;

    if (patternState == 1) {
      SetupYellowAndGreenPalette();
      currentBlending = LINEARBLEND;
    }
    if (patternState == 2) {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if (patternState == 3) {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if (patternState == 4) {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if (patternState == 5) {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }
    if (patternState == 6) {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
    }
    if (patternState == 7) {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
    }
    if (patternState == 8) {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if (patternState == 9) {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
    if (patternState == 10) {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = NOBLEND;
    }
    if (patternState == 11) {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = LINEARBLEND;
    }
    if (patternState >= 12) {
      ClearStrips();
      currentBlending = LINEARBLEND;
      patternState = 0;
      counterState = 0;
    }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette() {
  for (int i = 0; i < 16; ++i) {
    currentPalette[i] = CHSV(random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette() {
  // 'black out' all 16 palette entries...
  fill_solid(currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;
}

// This function sets up a palette of yellow and green stripes.
void SetupYellowAndGreenPalette() {
  CRGB yellow = CHSV(HUE_YELLOW, 255, 255);
  CRGB green = CHSV(HUE_GREEN, 255, 255);

  currentPalette = CRGBPalette16(
    green, green,
    yellow, yellow,
    green, green,
    yellow, yellow,
    green, green,
    yellow, yellow,
    green, green,
    yellow, yellow);
}

// This function sets up a palette of purple and green stripes.
void ClearStrips() {
  CRGB black = CRGB::Black;

  currentPalette = CRGBPalette16(
    black, black,
    black, black,
    black, black,
    black, black,
    black, black,
    black, black,
    black, black,
    black, black);
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p FL_PROGMEM = {
  CRGB::Red,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};


