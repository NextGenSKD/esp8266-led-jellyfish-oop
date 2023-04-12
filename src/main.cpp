#include <Arduino.h>
#define LED_PIN 5
#define LED_NUM 100
#include <FastLED.h>

CRGB leds[LED_NUM];
struct led_RGB
{
  byte r;
  byte g;
  byte b;
};

class o_leds
{
protected:
  led_RGB lds;

protected:
  led_RGB cols[2];

public:
  int _x, _y;

public:
  led_RGB *scr;

public:
  o_leds(int x, int y)
  {
    cols[0] = setRGB(120, 0, 120);
    cols[1] = setRGB(120, 0, 80);
    cols[2] = setRGB(80, 0, 120);
    scr = new led_RGB[x * y];
    _x = x;
    _y = y;
    
    for (int y1 = 0; y1 < _y; y1++)
    {
      for (int x1 = 0; x1 < _x; x1++)
      {
        setItem(x1,y1,setRGB(0,0,20));
      }
    }
  }

public:
  void scroll()
  {
    for (int x = _x; x > 0; x--)
    {
      for (int y = 0; y < _y; y++)
      {
        lds = getItem(x - 1, y);
        setItem(x, y, lds);
        setLow(20, 20, 20);
        setItem(x - 1, y, lds);
      }
    }
  }

public:
  void rnd()
  {
    byte rnd = random(10);
    byte y = random(_y);
    if (rnd < 4)
    {
      setItem(0, y, cols[rnd]);
    }
  }
  void setItem(int x, int y, led_RGB p)
  {
    scr[x * _y + y] = p;
  }
  led_RGB getItem(int x, int y)
  {
    return scr[x * _y + y];
  }

public:
  void setLow(byte r, byte g, byte b)
  {
    lds.r = lds.r > 20 ? lds.r - r : 0;
    lds.g = lds.g > 20 ? lds.g - g : 0;
    lds.b = lds.b > 40 ? lds.b - b : 20;
  }

public:
  led_RGB setRGB(byte r, byte g, byte b)
  {
    led_RGB tmp;
    tmp.r = r;
    tmp.g = g;
    tmp.b = b;
    return tmp;
  }
};

int counter = 0;

void debug()
{
  // Debug info
  Serial.print("\r Counter:");
  Serial.print(counter);
  Serial.print("   ");
  // End Debug
}

o_leds *l1 = new o_leds(8, 11);

void setup()
{
  Serial.begin(921600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(20);
  Serial.println("\r SETUP Success!");
}

void loop()
{

  l1->rnd();
  l1->scroll();

  for (int y = 0; y <= l1->_y; y++)
  {
    for (int x = 0; x <= l1->_x; x++)
    {
      led_RGB p = l1->getItem(x, y);
      leds[y * l1->_x + x] = ((p.b) + (p.r << 8) + (p.g << 16));
    }
  }

  FastLED.show();
  delay(50);
  debug();
  // FastLED.clear();
  counter++;
}