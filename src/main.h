#define VERBOSE_OUTPUT true

#define PIN_IN_BUTTON_RESET  2 //purple
#define PIN_IN_KNOB_SENSITIVITY  39 //blue
#define PIN_IN_SENSOR  34 //grey

#define PIN_OUT_S0  27 //yellow
#define PIN_OUT_S1  25 //green
#define PIN_OUT_S2  32 //blue
#define PIN_OUT_S3  12 //violet

#define PIN_OUT_NEOPIXEL 4  

#define READINGS_COUNT  4
#define TOTAL_FSRS  16
#define BRIGHTNESS_WHITE  60
#define NEOPIXEL_COUNT  64
#define KNOB_READINGS_COUNT 8
#define RED_HUE 55000
#define BLUE_HUE 42000

float threshold = 0.33;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel neopixelStrip = Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN_OUT_NEOPIXEL, NEO_GRB + NEO_KHZ800);

#include <BleGamepad.h>
BleGamepad bleGamepad("DDR Pad", "Grey Skies Automation", 69);

#include <Arduino.h>
#include "FSRSensor.h"
#include "Multiplexer.h"
#include "ArrowPanel.h"
#include <cstdint>

//#include <Joystick.h>




float endCutoff = 0.02;
float thresholdMinimum = 0.01;
float thresholdMaximum = 0.99;

const uint8_t holdLength = NEOPIXEL_COUNT;



uint8_t KnobReadings[KNOB_READINGS_COUNT];
uint8_t knobReadingIndex = 0;

int accumulatedButtonHold = 0;

uint8_t KnobChangeTimeout = 0;
uint8_t KnobChangeDisplayLengthTicks = 32;

uint8_t KnobReadingAverage = 0;


bool isInitialized = false;