#pragma once

#include <Arduino.h>

// How many leds are in the strip?
#define NUM_LEDS 95

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN        6
//#define CLOCK_PIN 13

#define FORCE_SENSOR_1  A0
#define FORCE_SENSOR_2  A1

#define TRIGGER_LEVEL   100   // int over which a trigger is set

#define SPEED_MS        5    // The wait (in mS) between moving to next step

#define FLARE_SIZE      10     // Number of LEDs to light in a block
#define FADE_STEP       30     // Fade in the final colour

#define START_COLOUR_R    0      
#define START_COLOUR_G    0      
#define START_COLOUR_B    255

#define END_COLOUR_R      255
#define END_COLOUR_G      255
#define END_COLOUR_B      255
