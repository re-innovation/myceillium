

#include <FastLED.h>
#include "config.h"

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

int force_sensor_1;
int force_sensor_2;

bool triggered = false;
bool clear_flag = false;
long int check_sensor_time = millis();
int led_start_pos = 0;
long int update_leds_time = millis();

// need to save the colour of the LEDs


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
  clear_leds();
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

  // need to slow down the display (control the speed of it)
  if (millis() >= update_leds_time + SPEED_MS)
  {
    if (triggered)
    {
      if (led_start_pos < NUM_LEDS)
      {
        // Move a single white led
        for (int Led = 0; Led < NUM_LEDS; Led = Led + 1)
        {
          // update all the LEDS:
          if (Led <= led_start_pos && Led > (led_start_pos - FLARE_SIZE))
          {
            leds[Led] = CRGB(START_COLOUR_R, START_COLOUR_G, START_COLOUR_B);
            // here we set the first LED bright, then the next dimmer etc

            //leds[i].setBrightness()
          }
          else
          {
            leds[Led] = CRGB::Black;
          }
        }
        led_start_pos++;
      }
      else
      {
        //      // we are at max position so show all as white
        //      for (int Led = 0; Led < NUM_LEDS; Led = Led + 1)
        //      {
        //        leds[Led] = CRGB::White;
        //      }
      }
      // Show the leds
      FastLED.show();
    }
    update_leds_time = millis();
  }
}

void check_sensors()
{
  // Here we test both the pressure sensors
  force_sensor_1 = analogRead(FORCE_SENSOR_1);
  force_sensor_2 = analogRead(FORCE_SENSOR_2);

  // for debug:
  //  Serial.print(force_sensor_1);
  //  Serial.print(" : ");
  //  Serial.println(force_sensor_2);

  if (force_sensor_1 > TRIGGER_LEVEL || force_sensor_2 > TRIGGER_LEVEL)
  {
    triggered = true;
    Serial.println("Press!");
    clear_flag = true;
  }
  else
  {
    triggered = false;
    Serial.println("");
    led_start_pos = 0;  // start at the beginning again
    // Also want to clear the LEDS
    if (clear_flag == true)
    {
      clear_leds();
      clear_flag = false;  // only do this once
    }
  }
}


void clear_leds()
{
  // this blanks all the LEDs
  for (int Led = 0; Led < NUM_LEDS; Led = Led + 1)
  {
    leds[Led] = CRGB::Black;
  }
  FastLED.show();
}
