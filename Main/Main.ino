/* 
 *  
 *  Creator : Thomas Cibils
 *  Last update : 05.02.2019
 *  FastLED tuto : https://github.com/FastLED/FastLED/wiki/Basic-usage - used for WS2812B 5050 RGB LED Strip 5M 150 300 Leds 144 60LED/M Individual Addressable 5V
 *  */

#include <TimerOne.h>
#include "FastLED.h"
#include <avr/pgmspace.h>

// LED MATRIX CODE
#define displayNumberOfRows 10                          // Number of rows
#define displayNumberOfColumns 6                       // Number of coumns
#define NUM_LEDS displayNumberOfRows * displayNumberOfColumns // Number of LEDs

CRGB leds[NUM_LEDS];                                          // Defining leds table for FastLed
#define DATA_PIN 6                                            // Output pin for FastLed

/* Constantes des bits de chaque bouton */
#define BTN_A 256
#define BTN_B 1
#define BTN_X 512
#define BTN_Y 2
#define BTN_SELECT 4
#define BTN_START 8
#define BTN_UP 16
#define BTN_DOWN 32
#define BTN_LEFT 64
#define BTN_RIGHT 128
#define BTN_L 1024
#define BTN_R 2048
#define NO_GAMEPAD 61440
 
/* Pin mapping */
static const byte PIN_LATCH = 2;
static const byte PIN_CLOCK = 3;
static const byte PIN_DATA = 4;

byte leftButtonPushed = 0;
byte rightButtonPushed = 0;
byte upButtonPushed = 0;
byte downButtonPushed = 0;
byte aButtonPushed = 0;
byte bButtonPushed = 0;
byte xButtonPushed = 0;
byte yButtonPushed = 0;
byte rButtonPushed = 0;
byte lButtonPushed = 0;
byte startButtonPushed = 0;
byte selectButtonPushed = 0;


// LED Matrix
// top column is from 0 to 7, bottom one from 56 to 63 (for a 8x8 matrix)
byte LEDMatrix[displayNumberOfRows][displayNumberOfColumns];


// Original colours for leds.
const byte Black = 0;
const byte White = 1;
const byte Blue = 2;
const byte Red = 3;
const byte Green = 4;
const byte Purple = 5;

struct pointOnMatrix {
  byte lineCoordinate;
  byte columnCoordinate;
};

unsigned long lastMillis = 0;
unsigned const int growthSpeed = 1500;  // In miliseconds, every how much will the menace grow




void setup() {

  Serial.begin(9600);

  // Set matrix pins to output
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  /* Initialisation des broches */
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, INPUT); 
}

void loop() {

if(lastMillis - millis() > 500) {


  lastMillis = millis();
}

   
   checkButtons();      // Checks which buttons have been pushed and set according variable to 1

  // Do stuff using buttons variables here

   resetButtons();      // Re-sets button checks to 0 for the next loop

  outputDisplay();
  delay(1);
}

void checkButtons() {
  
    static uint16_t oldBtns = 0;      // Anciennes valeurs des boutons
    uint16_t btns = getSnesButtons(); // Valeurs actuelles des boutons
    
    /* Affiche l'état des boutons uniquement en cas de changement */
    if(oldBtns != btns) {oldBtns = btns;}
    else                {return;}
  
    if(btns & NO_GAMEPAD) {
      Serial.println(F("No gamepad connected"));
      return;
    }
     
    /* Affiche l'état de chaque bouton */
    if(btns & BTN_A) {
      aButtonPushed = 1;
    }
        
    if(btns & BTN_B){
      bButtonPushed = 1;
    }
  
    
    if(btns & BTN_X) {
      xButtonPushed = 1;
    }
   
    if(btns & BTN_Y) {
      yButtonPushed = 1;
    }

    if(btns & BTN_SELECT) {
      selectButtonPushed = 1;
    }
   
    if(btns & BTN_START) {
      startButtonPushed = 1;
    }
   
    if(btns & BTN_UP) {
      upButtonPushed = 1;
    }
    
    if(btns & BTN_DOWN) {
      downButtonPushed = 1;
    }
    
    if(btns & BTN_LEFT) {
      leftButtonPushed = 1;
    }
   
    if(btns & BTN_RIGHT){
      rightButtonPushed = 1;
    }
     
    if(btns & BTN_L) {
      lButtonPushed = 1;
    }
   
    if(btns & BTN_R) {
      rButtonPushed = 1;
    }
}

void resetButtons() {
  leftButtonPushed = 0;
  rightButtonPushed = 0;
  upButtonPushed = 0;
  downButtonPushed = 0;
  aButtonPushed = 0;
  bButtonPushed = 0;
  xButtonPushed = 0;
  yButtonPushed = 0;
  rButtonPushed = 0;
  lButtonPushed = 0;
  startButtonPushed = 0;
  selectButtonPushed = 0;
}

/** Retourne l'état de chaque bouton sous la forme d'un entier sur 16 bits. */
uint16_t getSnesButtons() {
 
  /* 1 bouton = 1 bit */
  uint16_t value = 0;
 
  /* Capture de l'état courant des boutons */
  digitalWrite(PIN_LATCH, HIGH);
  digitalWrite(PIN_LATCH, LOW);
 
  /* Récupère l'état de chaque bouton (12 bits + 4 bits à "1") */
  for(byte i = 0; i < 16; ++i) {
 
    /* Lit l'état du bouton et décale le bit reçu pour former l'entier sur 16 bits final */
    value |= digitalRead(PIN_DATA) << i;
 
    /* Pulsation du signal d'horloge */
    digitalWrite(PIN_CLOCK, HIGH);
    digitalWrite(PIN_CLOCK, LOW);
  }
 
  /* Retourne le résultat sous une forme facile à manipuler (bouton appuyé = bit à "1") */
  return ~value;
}


// Makes the whole "LEDMatrix" equals to 0, i.e. each LED is off
void clearLEDMatrix() {
  // Just seting le LEDmatrix to Wall
  for (byte i = 0; i < displayNumberOfRows; i++)  {
    for (byte j = 0; j < displayNumberOfColumns; j++) {
      LEDMatrix[i][j] = Black;
    }
  }
}


// We update the physical display of the LED matrix, based on the LEDMatrix
void outputDisplay() {
  for(byte rowIndex = 0; rowIndex < displayNumberOfRows; rowIndex++) {
    for(byte columnIndex = 0; columnIndex < displayNumberOfColumns; columnIndex++) {
      // Useful because of the way my matrix is soldered.
      // So we'll invert one column every two compared to our digital matrix
      // If we're on an even column, we're fine, everything is straightfoward
      if(columnIndex%2 == 0) {
        
        if(LEDMatrix[rowIndex][columnIndex] == Black) {leds[(columnIndex + 1)*displayNumberOfRows - rowIndex - 1] = CRGB::Black;}
        if(LEDMatrix[rowIndex][columnIndex] == White) {leds[(columnIndex + 1)*displayNumberOfRows - rowIndex - 1] = CRGB::White;}
        if(LEDMatrix[rowIndex][columnIndex] == Green) {leds[(columnIndex + 1)*displayNumberOfRows - rowIndex - 1] = CRGB::Green;}
        if(LEDMatrix[rowIndex][columnIndex] == Blue) {leds[(columnIndex + 1)*displayNumberOfRows - rowIndex - 1] = CRGB::Blue;}
        if(LEDMatrix[rowIndex][columnIndex] == Red) {leds[(columnIndex + 1)*displayNumberOfRows - rowIndex - 1] = CRGB::Red;}
        if(LEDMatrix[rowIndex][columnIndex] == Purple) {leds[(columnIndex + 1)*displayNumberOfRows - rowIndex - 1] = CRGB::Purple;}
      }
      // If we're on an uneven column, we do a mathematical trick to invert it
      else if(columnIndex%2 == 1) {
        if(LEDMatrix[rowIndex][columnIndex] == Black) {leds[columnIndex*displayNumberOfRows + rowIndex] = CRGB::Black;}
        if(LEDMatrix[rowIndex][columnIndex] == White) {leds[columnIndex*displayNumberOfRows + rowIndex] = CRGB::White;}
        if(LEDMatrix[rowIndex][columnIndex] == Green) {leds[columnIndex*displayNumberOfRows + rowIndex] = CRGB::Green;}
        if(LEDMatrix[rowIndex][columnIndex] == Blue) {leds[columnIndex*displayNumberOfRows + rowIndex] = CRGB::Blue;}
        if(LEDMatrix[rowIndex][columnIndex] == Red) {leds[columnIndex*displayNumberOfRows + rowIndex] = CRGB::Red;}
        if(LEDMatrix[rowIndex][columnIndex] == Purple) {leds[columnIndex*displayNumberOfRows + rowIndex] = CRGB::Purple;}
      }
    }
  }
  
  // Display the matrix physically
  FastLED.show(); 
}

// We update the digital display of the LED matrix
void digitalOutputDisplay() {
  Serial.print("\n We print digitally the current theoritical state of the LED Matrix : \n");
  for (byte i = 0; i < displayNumberOfRows; i++) {
    for (byte j = 0; j < displayNumberOfColumns; j++) {
      Serial.print(LEDMatrix[i][j]);
      if (j < displayNumberOfColumns - 1) {
        Serial.print(", ");
      }
      else {
        Serial.print("\n");
      }
    }
  }
}
