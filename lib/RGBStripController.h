#include "Definitions.h"
#include <Adafruit_NeoPixel.h>

#ifndef RGBStripController_h
#define RGBStripController_h

class RGBStripController
{
private:
  RGB color;
  int pin;
  int qtyLeds;
  Adafruit_NeoPixel pixels;

public:
  RGBStripController(int pin, int qtyLeds)
  {
    this->pin = pin;
    this->qtyLeds = qtyLeds;
    this->pixels = Adafruit_NeoPixel(qtyLeds, pin, NEO_GRB + NEO_KHZ800);

    this->color = RGB{0, 0, 0};
    this->fadeColor(RGB{255, 255, 255}, 10);
  }

  void begin()
  {
    pinMode(this->pin, OUTPUT);
  }

  void setColor(RGB color)
  {
    this->color = color;
    this->pixels.clear();

    for (int i = 0; i < (this->qtyLeds); i++)
      this->pixels.setPixelColor(i, pixels.Color(this->color.R, this->color.G, this->color.B));

    this->pixels.show();
  }

  void fadeColor(RGB color, unsigned int delayTime)
  {
    this->pixels.clear();

    unsigned short dR = (this->color.R - color.R) / 100;
    unsigned short dG = (this->color.G - color.G) / 100;
    unsigned short dB = (this->color.B - color.B) / 100;

    for (unsigned short animation = 1; animation <= 100; animation++)
    {
      setColor(RGB{
        (unsigned short)((this->color.R + dR * animation) % 255),
        (unsigned short)((this->color.G + dG * animation) % 255),
        (unsigned short)((this->color.B + dB * animation) % 255)
      });

      this->pixels.show();
      delay(delayTime / 100);
    }
  }

  void tick() {}
};

#endif
