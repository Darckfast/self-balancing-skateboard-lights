#include <Arduino.h>
#include <VescUart.h>
#include <FastLED.h>

#define NUM_LEDS_BATTERY 10
#define NUM_LEDS_TAIL_LIGHT 22
#define NUM_LEDS_HEAD_LIGHT 11

#define DATA_PIN_BATTERY 4
#define DATA_PIN_TAIL_LIGHT 5
#define DATA_PIN_HEAD_LIGHT 3

VescUart UART;

CRGB leds_battery[NUM_LEDS_BATTERY];
CRGB leds_tail_light[NUM_LEDS_TAIL_LIGHT];
CRGB leds_head_light[NUM_LEDS_HEAD_LIGHT];

void light_up_head_light(int mode);
void light_up_tail_light(int mode);
void light_up_battery_indicator(float batteryPercent);
void light_up_amp_indicator(float ampPercent);

float batteryPercent = 0;
unsigned long switchTime = 0l;

void setup()
{
  // delay(10000); // wait for vesc to initiate

  FastLED.addLeds<NEOPIXEL, DATA_PIN_BATTERY>(leds_battery, NUM_LEDS_BATTERY);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_TAIL_LIGHT>(leds_tail_light, NUM_LEDS_TAIL_LIGHT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_HEAD_LIGHT>(leds_head_light, NUM_LEDS_HEAD_LIGHT);

  // Serial1.begin(115200);

  // while (!Serial1)
  // {
  //   ;
  // }

  // UART.setSerialPort(&Serial1);

  switchTime = millis();
}

void loop()
{
  // if (!UART.getVescValues() || batteryPercent < 0)
  // {

  //   light_up_tail_light(-1);
  //   light_up_head_light(-1);
  //   light_up_battery_indicator(-1);
  // }
  // else
  // {
  //   batteryPercent = ((UART.data.inpVoltage - 32) / 10) * 100;

  //   light_up_battery_indicator(batteryPercent);
  //   light_up_tail_light(UART.data.rpm != 0);
  //   light_up_head_light(UART.data.rpm != 0);
  // }

  float amp_percentage = /*UART.data.ampHours*/ (4.0 / 10) * 100;

  // light_up_battery_indicator(80);

  light_up_amp_indicator(amp_percentage);
  light_up_tail_light(0);
  light_up_head_light(0);

  delay(200);
}

void switch_between_amp_and_battery()
{
  if ((switchTime - millis()) % 2 == 0)
  {
    switchTime = millis();
  }
}

void light_up_battery_indicator(float batteryPercent)
{
  for (int i = 0; i < NUM_LEDS_BATTERY; i++)
  {

    leds_battery[i] = i * 10 <= batteryPercent ? CHSV(0, 255, 48) : CHSV(0, 0, 0);

    FastLED.show();
  }
}

void light_up_amp_indicator(float ampPercent)
{
  for (int i = 0; i < NUM_LEDS_BATTERY; i++)
  {

    leds_battery[10 - i] = i * 10 <= ampPercent ? CHSV(16, 255, 48) : CHSV(0, 0, 0);

    FastLED.show();
  }
}

void light_up_head_light(int mode)
{
  for (int i = 0; i < NUM_LEDS_HEAD_LIGHT; i++)
  {

    leds_head_light[i] = CHSV(0, 0, 0);

    if (mode == 0)
    {
      leds_head_light[i] = CHSV(160, 100, 96);
    }
    else if (mode > 0)
    {
      leds_head_light[i] = CRGB(200, 200, 200);
    }

    FastLED.show();
  }
}

void light_up_tail_light(int mode)
{
  for (int i = 3; i < NUM_LEDS_TAIL_LIGHT; i++)
  {

    leds_tail_light[i] = CHSV(0, 0, 0);

    if (i == 7 || i == 14)
    {
      continue;
    }

    if (mode == 0)
    {
      leds_tail_light[i] = CHSV(0, 255, 16);
    }
    else if (mode > 0)
    {
      leds_tail_light[i] = CHSV(0, 255, 64);
    }

    FastLED.show();
  }
}
