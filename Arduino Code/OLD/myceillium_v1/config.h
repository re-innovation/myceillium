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
