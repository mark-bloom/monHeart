#include <FastLED.h>

// ** FastLED definitions **
#define LED_PIN       4
#define NUM_LEDS      150
#define BRIGHTNESS    128
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB

CRGB leds[NUM_LEDS];

// ** Program definitions **
#define FADETIME 6000
// PULSE Definitions
#define PULSESPD 1 // def (1)
#define PULSECNT 2 // def (3)

// ** GLOBAL VARIABLES **
int secondHand;
int counter;
unsigned long currentTime;
unsigned long relCurrentTime;
unsigned long previousTime;
unsigned long threshold;
bool firstRun;
bool toggleCount;
uint8_t globalIndex;

//pulse
uint8_t speedCount;
uint8_t brightVal;
bool reverse;

// Temp Palette used
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;
#define MAX_CHANGES   10  //10 for 5 seconds and 5m at +1 speed

// ** COLOR PALETTES **
// Color Palette: Rosa PurpleBlueAqua palette
extern CRGBPalette16 PurpleBlueAqua;
extern const TProgmemPalette16 PurpleBlueAqua_p PROGMEM;
// Color Palette: Rosa YellowOrangePink palette
extern CRGBPalette16 YellowOrangePink;
extern const TProgmemPalette16 YellowOrangePink_p PROGMEM;
// Color Palette: Mark Custom2 palette
extern CRGBPalette16 MHeart;
extern const TProgmemPalette16 MHeart_p PROGMEM;

CRGBPalette16 M1Palette_p;

// *************************
// ** SETUP **
// *************************

void setup() {
  delay( 1000 );  // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  clearAllNA();

  relCurrentTime = 0;
  threshold = 300000; //ms 5 minutes
  firstRun = true;
  toggleCount = false;
  globalIndex = 0;
  counter = 0;

  setupM1Palette();
  previousTime = millis();
}

// *************************
// ** MAIN LOOP **
// *************************

void loop() {
  currentTime = millis();
  relCurrentTime = currentTime - previousTime;
  static uint8_t selectedEffect = 5; // 10 = FADE OUT // 5 = OPENING PULSE // 0 = PALETTE
  static uint8_t tempEffect = 0;

  // Run next program after fade-out
  if ((selectedEffect == 5) && (relCurrentTime > FADETIME)){
      previousTime = currentTime;
      selectedEffect = tempEffect + 1;
      if(selectedEffect > 1 && selectedEffect != 5){
        selectedEffect = 0;
      }
      firstRun = true;
      //clearAllNA();
  }
  
  // Run fade-out program from treshold
  if(relCurrentTime > (threshold - FADETIME)){
      tempEffect = selectedEffect;
      previousTime = currentTime;
      counter = 0;
      selectedEffect = 10; // Run fade out program
      firstRun = true;
  }

  switch(selectedEffect) {
      // COLOR PALETTE
      case 0  : {
                  if(firstRun){
                      currentPalette = RainbowColors_p;
                      targetPalette = OceanColors_p;
                      currentBlending = LINEARBLEND;
                      brightVal = 255;
                      threshold = 860000; //SET TO 30s FOR TESTING 290000
                      firstRun = false;
                  }
                  runColorPalette();
                  break;
                }
      // PULSE PALETTE
      case 1  : {
                  if(firstRun){
                      currentPalette = MHeart_p;
                      currentBlending = LINEARBLEND;
                      brightVal = 255;
                      speedCount = 0;
                      threshold = 120000;
                      firstRun = false;
                  }
                  runPulseProg();
                  break;
                }
//      case 2  : {
//                  if(firstRun){
//                      firstRun = false;
//                  }
//                  break;
//                }
//      case 3  : {
//                  if(firstRun){
//                      firstRun = false;
//                  }
//                  break;
//                }
//      case 4  : {
//                  break;
//                }
      // OPENING PULSE
      case 5  : {
                  if(firstRun){
                      currentPalette = MHeart_p;
                      currentBlending = LINEARBLEND;
                      brightVal = 255;
                      speedCount = 0;
                      threshold = 30000;
                      firstRun = false;
                  }
                  runPulseProg();
                  break;
                }
//      case 6  : {
//                  if(firstRun){
//                      firstRun = false;
//                  }
//                  break;
//                }
//      case 7  : {
//                  if(firstRun){
//                      firstRun = false;
//                  }
//                  break;
//                }
//      case 8  : {
//                  if(firstRun){
//                      firstRun = false;
//                  }
//                  break;
//                }
//      //TEST
//      case 9  : { 
//                  if(firstRun){
//                      firstRun = false;
//                  }
//                  break;
//                }
      case 10 : {
                  if(firstRun){
                    if(tempEffect == 0 || tempEffect == 4){
                      targetPalette = MHeart_p;
                    }
                    else if(tempEffect == 1 || tempEffect == 5){
                      targetPalette = RainbowColors_p;
                    }
                    firstRun = false;
                  }
                  nblendPaletteTowardPalette( currentPalette, targetPalette, 5);
                  FillLEDsFromPaletteColors();
                  FastLED.show();
                  break;
                }
  }
}

// *************************
// ** FUNCTIONS **
// *************************

// COLOR PALETTE
void runColorPalette() {
    secondHand = (relCurrentTime / 1000) % (440);
    if( secondHand == 0)        { currentPalette = RainbowColors_p;      currentBlending = LINEARBLEND; 
                                        targetPalette = OceanColors_p;}
    else if( secondHand == 80)  { currentPalette = OceanColors_p;        currentBlending = LINEARBLEND; 
                                        targetPalette = PartyColors_p;}
    else if( secondHand == 160)  { currentPalette = PartyColors_p;        currentBlending = LINEARBLEND; 
                                        targetPalette = PurpleBlueAqua_p;}
    else if( secondHand == 240)  { currentPalette = PurpleBlueAqua_p;       currentBlending = LINEARBLEND; 
                                        targetPalette = HeatColors_p;}
    else if( secondHand == 320)  { currentPalette = HeatColors_p;        currentBlending = LINEARBLEND; 
                                        targetPalette = YellowOrangePink_p;}
    else if( secondHand == 400)  { currentPalette = YellowOrangePink_p;    currentBlending = LINEARBLEND; 
                                        targetPalette = RainbowColors_p;}
    if( secondHand % 80 >= 75){
        nblendPaletteTowardPalette( currentPalette, targetPalette, MAX_CHANGES);
    }
    if(counter%2 == 0){ // half speed
      globalIndex = globalIndex + 1; // speed of wave motion and color transitions
    }
    counter++;
    FillLEDsFromPaletteColorsDouble();
    FastLED.show();
}
void FillLEDsFromPaletteColors() {
    uint8_t tempIndex1 = globalIndex;
    uint8_t brightness = 255;
    for( uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, tempIndex1, brightness, currentBlending);
        tempIndex1 = tempIndex1 - 2;
        if(i==0) {
          globalIndex = globalIndex - 2;
        }
    }
}

void FillLEDsFromPaletteColorsDouble() {
    uint8_t tempIndex1 = globalIndex;
    uint8_t brightness = 255;
    for( uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, tempIndex1, brightness, currentBlending);
        tempIndex1 = tempIndex1 - 5;
        if(i==0) {
          globalIndex = globalIndex - 5;
        }
    }
}

void FillLEDsFromPaletteColorsPulse( uint8_t brightness) {
    uint8_t tempIndex1 = globalIndex;
    for( uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, tempIndex1, brightness, currentBlending);
        tempIndex1 = tempIndex1 + 5;
        if(i==0) {
          globalIndex = globalIndex + 5;
        }
    }
}

// PULSE
void runPulseProg() {
  runPulse();
}
void runPulse() {
    secondHand = (relCurrentTime / 1000) % 600;
    if( secondHand < 120 ){
        targetPalette = MHeart_p;
        pulse();
// //COMMENT SINCE NO TRANSITIONS
//    } else if( secondHand < 240 ){
//        targetPalette = MHeart_p;//YellowOrangePink_p;
//        pulse();
//    } else if( secondHand < 360 ){
//        targetPalette = MHeart_p;//OceanColors_p;
//        pulse();
//    } else if( secondHand < 480 ){
//        targetPalette = MHeart_p;//ForestColors_p;
//        pulse();
//    } else if( secondHand < 600 ){
//        targetPalette = MHeart_p;//PurpleBlueAqua_p;
//        pulse();
//    }
//    if (secondHand%120 > 105) {
//        nblendPaletteTowardPalette( currentPalette, targetPalette, MAX_CHANGES);
    }
    FastLED.show();
}

void pulse(){
      FillLEDsFromPaletteColorsPulse(brightVal);
      if(counter%5 == 0){ // one fifth speed
        globalIndex = globalIndex + 1; // speed of wave motion and color transitions
      }
      counter++;
      if(brightVal > 250) {
          delay(350);
          reverse = true;
      } else if (brightVal < 80) {
          reverse = false;
      }
      if(speedCount < 1){
        if(reverse){
            brightVal = brightVal - PULSESPD;
        } else {
            brightVal = brightVal + PULSESPD;
        }
      }
      speedCount++;
      if(speedCount == PULSECNT){
        speedCount = 0;
      }
}
void changePulsePal(uint8_t strip, CRGBPalette16 targetPalette){
    if(brightVal == 0) {
        currentPalette = targetPalette;
    }
    else {
        brightVal = brightVal - PULSESPD;
    }
}

// *************************
// ** CUSTOM PALETTES **
// *************************

// Rosa Custom Palette PurpleBlueAqua COLOR PALETTE
const TProgmemPalette16 PurpleBlueAqua_p PROGMEM = {
    CRGB::Aqua,
    CRGB::Amethyst,
    CRGB::Coral,
    CRGB::MistyRose,
    
    CRGB::Plum,
    CRGB::PowderBlue,
    CRGB::SkyBlue,
    CRGB::Turquoise,
    
    CRGB::Coral,
    CRGB::MistyRose,
    CRGB::Seashell,
    CRGB::Sienna,
    
    CRGB::Aqua,
    CRGB::Amethyst,
    CRGB::Coral,
    CRGB::MistyRose,
};

// Rosa Custom Palette YellowOrangePink COLOR PALETTE
const TProgmemPalette16 YellowOrangePink_p PROGMEM = {
    CRGB::Gold,
    CRGB::Gold,
    CRGB::Orange,
    CRGB::Orange,
    
    CRGB::OrangeRed,
    CRGB::OrangeRed,
    CRGB::Purple,
    CRGB::Purple,
    
    CRGB::Purple,
    CRGB::Purple,
    CRGB::OrangeRed,
    CRGB::OrangeRed,
    
    CRGB::Orange,
    CRGB::Orange,
    CRGB::Gold,
    CRGB::Gold,
};

// Mark Custom Palette MHeart COLOR PALETTE
const TProgmemPalette16 MHeart_p PROGMEM = {
    CRGB::Red,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::Maroon,
    
    CRGB::DarkRed,
    CRGB::Red,
    CRGB::Purple,
    CRGB::Purple,
    
    CRGB::Red,
    CRGB::DarkRed,
    CRGB::Maroon,
    CRGB::DarkRed,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Purple,
    CRGB::Red,
};

// *************************
// ** COMMON FUNCTIONS **
// *************************
//
void setupM1Palette(){
    M1Palette_p = CRGBPalette16(
    0x000000, 0x660000, 
    0xCC0000, 0xFF6600,
    0xFFCC00, 0xFFFF00,
    0xFFFF66, 0xFFFFFF,

    0xFFFFFF, 0xFFFF99,
    0xFFFF33, 0xFFFF00,
    0xFF9900, 0xFF0000, 
    0x990000, 0x000000
    );
}

void fadeOut(int fadeVal) {
    for(uint8_t i = 0; i < NUM_LEDS; i++){
        leds[i].fadeToBlackBy( fadeVal );
    }
    FastLED.show();
}

// Clear All functions
// Set all LEDs to OFF and apply it (visible)
void clearAll() {
    for(uint8_t i = 0; i < NUM_LEDS; i++ ) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}
// Set all LEDs to OFF but don't apply it
void clearAllNA() {
    for(uint8_t i = 0; i < NUM_LEDS; i++ ) {
        leds[i] = CRGB::Black;
    }
}

// Set All functions
// Set all LEDs to a given color and apply it (visible)
void setAll(byte red, byte green, byte blue) {
    for(uint8_t i = 0; i < NUM_LEDS; i++ ) {
        leds[i] = CRGB(red,green,blue);
    }
    FastLED.show();
}
// Set all LEDs to a given color but don't apply it
void setAllNA(byte red, byte green, byte blue) {
    for(uint8_t i = 0; i < NUM_LEDS; i++ ) {
        leds[i] = CRGB(red,green,blue);
    }
}
