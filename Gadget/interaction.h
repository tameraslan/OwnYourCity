#ifndef INTERACTION_H
#define INTERACTION


// OLED
#if OLED
#include <Wire.h> // SCL: D1 (GPIO5); SDA: D2 (GPIO4)


#include "src/Adafruit_SSD1306-esp8266-64x48/Adafruit_SSD1306.h"
#include "src/OLED_BUTTON/src/LOLIN_I2C_BUTTON.h" // Buttons: A: D3 (GPIO0); B: D4 (GPIO2)
#include <Adafruit_GFX.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
I2C_BUTTON button(DEFAULT_I2C_BUTTON_ADDRESS); //I2C Address 0x31

bool ButtonClickedB = false;
#endif

bool uploadRequest;

#include "FastLED.h"

#define NUM_LEDS 5


#define DATA_PIN 14


// Define the array of leds
CRGB leds[NUM_LEDS];
void initInteractions()
{
  uploadRequest = false;
  if (OLED)
  {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
    display.display();

    display.clearDisplay();
    pinMode(D4, INPUT); // Setting D4 B Button as input
    pinMode(D3, INPUT); // Setting D3 A Button as input

  }
  //if(BUTTON)

  if (LED)
  {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  }

  if (DEBUG) Serial.println("Interaction methods set up");
}


void processInteractions()
{
  if (OLED)
  {

    // handle buttons
    if (digitalRead(D3) == LOW) //A
    {
      if(SOLAR)
      {
        resetCharge = true;
        }
      if(NOISE)
      gameOver = false;

      
    }
    if (digitalRead(D4) == LOW) //B
    {

      uploadRequest = true;
      ButtonClickedB = true;

    }
    //display values
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    if (offline) display.println("OFFLINE");

    // DISPLAY
    if (uploadRequest)
    {
      // show upload screen

      display.setTextSize(1);
      display.println("Uploading");
      if (SOLAR)
      {
        display.println("Charge:");
        display.setTextSize(2);
        display.println(chargedWattSec);
        display.setTextSize(1);
        display.println("Ws");
      }
      if (NOISE)
      {
        display.println("NoiseLevel");
        display.setTextSize(2);
        display.print(noise);
        //display.println("dB");
      }
      if (AIR)
      {
        //display.println("AirQuality");
        //display.setTextSize(2);

        display.print(ppm);
        display.println("ppm");
      }
      if (WIND)
      {
        //display.println("AirQuality");
        //display.setTextSize(2);

        display.print(sensorValue);
        //display.println("ppm");
      }
      display.display();
      ButtonClickedB = false;
    }
    else
    {
      // display sensor values, etc.

      //display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);

      if (SOLAR)
      {

        display.setTextSize(1);
        //        display.println("SolarVolt:");
        //        display.print(solarVoltage);
        //        display.println("V");
        //        display.println();
        //display.setTextColor(WHITE);
        display.println("Solar");
        display.println("Potential:");
        display.print(solarPower);
        display.println("W");
        display.println("Charged:");
        display.print(chargedWattSec);
        display.println(" Ws");


      }
      if (NOISE)
      {
        display.setTextSize(1);
        display.println("Noise");
        display.setTextSize(2);
        display.println(noise);
        display.println("Volts");

      }
      if (AIR)
      {

        display.print(airSensorValue);
        display.println("");
        display.print(airVoltage);
        display.println("V");

        //display.println("AirQuality");
        display.setTextSize(1);
        display.println((int)ppm);
        display.println("ppm");
        //        display.print(ppm_m1);
        //        display.println("ppm_m1");
      }
      if (WIND)
      {

        display.print(windSensorValue);
        display.println("");
        display.print(windVoltage);
        display.println("V");

      }

    }
    display.display();
  }

  if (LED)
  {
    if (SOLAR)
    {
      if (chargedWattSec > 0.0 && chargedWattSec < 0.5)
      {
        leds[0] = CRGB::Black;
        FastLED.show();
        delay(250);
        leds[0] = CRGB::Red;
      }
      if (chargedWattSec > 0.5 && chargedWattSec < 1.0)
      {
        // blinking red
        leds[0] = CRGB::Red;
        
      }
      else if (chargedWattSec >= 1.0 && chargedWattSec < 1.5)
      {
        leds[0] = CRGB::Yellow;
      }
      else if (chargedWattSec >= 1.5)
      {
        leds[0] = CRGB::Green;
      }
    }
    if (NOISE)
    {
      // 5 pixels, increase in time when its loud.

      if (!gameOver)
      {

        for (int t = 0; t < 5; t++)
        {

          if (t < noiseLimit) leds[t] = CRGB::Red;
          else  leds[t] = CRGB::Green;
        }


      }
      else
      {
        // blink red
        for (int t = 0; t < 5; t++) leds[t] = CRGB::Black;
        FastLED.show();
        delay(500);
        for (int t = 0; t < 5; t++) leds[t] = CRGB::Red;
      }

    }
    if (AIR)
    {
    }
    if (WIND)
    {
      if (windSensorValue > 0 && windSensorValue < 100)
      {
        leds[0] = CRGB::Red;
      }
      else if (windSensorValue >= 100 && windSensorValue < 200)
      {
        leds[0] = CRGB::Blue;
      }
      else if (windSensorValue >= 200 && windSensorValue < 400)
      {
        leds[0] = CRGB::Yellow;
      }
      else if (windSensorValue >= 400 && windSensorValue < 600)
      {
        leds[0] = CRGB::Green;
      }

      // mapping:
      //leds[0] = CRGB(0,0,map(windSensorValue,100,300,0,255));

    }

    FastLED.show();



  }
  //  if (BUTTON)
  //  {
  //
  //  }
}


#endif
