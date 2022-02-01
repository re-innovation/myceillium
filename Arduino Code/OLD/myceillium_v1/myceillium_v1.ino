

#include <FastLED.h>
#include "config.h"

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

int force_sensor_1;
int force_sensor_2;

bool triggered = false;
long int check_sensor_time = millis();

// This function sets up the ledsand tells the controller about them
void setup()
{
  Serial.begin(115200);

  // Uncomment/edit one of the following lines for your leds arrangement.
  // ## Clockless types ##
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

  // Always clear the strip first!
  for (int Led = 0; Led < NUM_LEDS; Led = Led + 1)
  {
    leds[Led] = CRGB::Black;
  }
  FastLED.show();
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop()
{
  // Check sensors every 50mS
  if (millis() > check_sensor_time + 50)
  {
    check_sensors();
    check_sensor_time = millis();
  }

  if (triggered)
  {
    Serial.println("sensor activated!");
    // Move a single white led
    for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::White;
      // Show the leds (only one of which is set to white, from above)
      FastLED.show();
      // Wait a little bit
      delay(20);
      // Turn our current led back to black for the next loop around
      leds[whiteLed] = CRGB::Black;
    }
  }

}

void check_sensors()
{
  // Here we test both the pressure sensors
  force_sensor_1 = analogRead(FORCE_SENSOR_1);
  force_sensor_2 = analogRead(FORCE_SENSOR_2);

  Serial.print(force_sensor_1);
  Serial.print(" : ");
  Serial.println(force_sensor_2);

  if (force_sensor_1 > TRIGGER_LEVEL || force_sensor_2 > TRIGGER_LEVEL)
  {
    triggered = true;
  }
  else
  {
    triggered = false;
  }
}
