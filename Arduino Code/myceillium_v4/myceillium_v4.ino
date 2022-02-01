

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
int led_end_fade = 0;

// need to save the colour of the LEDs
uint8_t colour_R;
uint8_t colour_G;
uint8_t colour_B;

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
      // Here we ewant to calculate the colour from the LED position
      // at 0 we want to be at start colour
      // at NUM_LEDS we want to be at end colour
      calculate_colour();

      if (led_start_pos < NUM_LEDS)
      {
        // Move a single white led
        for (int Led = 0; Led < NUM_LEDS; Led = Led + 1)
        {
          // update all the LEDS:
          if (Led <= led_start_pos && Led > (led_start_pos - FLARE_SIZE))
          {
            leds[Led] = CRGB(colour_R, colour_G, colour_B);
            // here we set the first LED bright, then the next dimmer etc
            float dim_value = ((((float)led_start_pos - (float)Led) / (float)FLARE_SIZE) * 255.0);
            leds[Led].fadeLightBy((int)dim_value);
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
        // Here we want to fade in the whole strip to white and keep it

        // we are at max position so show all as white
        for (int Led = 0; Led < NUM_LEDS - FLARE_SIZE; Led = Led + 1)
        {
          leds[Led] = CRGB(colour_R, colour_G, colour_B);
          leds[Led].fadeLightBy(255 - led_end_fade);
        }
        led_end_fade = led_end_fade + FADE_STEP;
        if (led_end_fade > 255)
        {
          led_end_fade = 255;
        }

        // keep the top flare lit (so does not flicker)
        for (int Led = NUM_LEDS - FLARE_SIZE; Led < NUM_LEDS; Led = Led + 1)
        {
          leds[Led] = CRGB(colour_R, colour_G, colour_B);
        }
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
    led_end_fade = 0;
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

void calculate_colour()
{
  if (led_start_pos > NUM_LEDS / FADE_OUT_POINT)
  {

    float ratio = ((float)led_start_pos - ((float)NUM_LEDS / FADE_OUT_POINT)) / ((float)NUM_LEDS - ((float)NUM_LEDS / FADE_OUT_POINT));

    float colour_R_float =   (float)START_COLOUR_R + ((float)END_COLOUR_R * ratio);
    if (colour_R_float >= 255)
    {
      colour_R_float = 255;
    }
    colour_R = uint8_t(colour_R_float);

    float colour_G_float =   (float)START_COLOUR_G + ((float)END_COLOUR_G * ratio);
    if (colour_G_float >= 255)
    {
      colour_G_float = 255;
    }
    colour_G = uint8_t(colour_G_float);

    float colour_B_float =   (float)START_COLOUR_B + ((float)END_COLOUR_B * ratio);
    if (colour_B_float >= 255)
    {
      colour_B_float = 255;
    }
    colour_B = uint8_t(colour_B_float);
  }
  else
  {
    colour_R = START_COLOUR_R;
    colour_G = START_COLOUR_G;
    colour_B = START_COLOUR_B;
  }
}
