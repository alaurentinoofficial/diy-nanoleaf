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
    this->fadeColor(&RGB{255, 255, 255}, 10);
  }

  void begin()
  {
    pinMode(this->pin, OUTPUT);
  }

  void setColor(RGB *color)
  {
    this->color = *color;
    this->pixels.clear();

    for (int i = 0; i < (this->qtyLeds); i++)
      this->pixels.setPixelColor(i, pixels.Color(this->color.R, this->color.G, this->color.B));

    this->pixels.show();
  }

  void fadeColor(RGB *color, unsigned int delayTime)
  {
    this->pixels.clear();

    int dR = (this->color.R - color->R) / 100;
    int dG = (this->color.G - color->G) / 100;
    int dB = (this->color.B - color->B) / 100;

    for (short animation = 1; animation <= 100; animation++)
    {
      setColor(new RGB{
          this->color.R + dR * animation, this->color.G + dG * animation, this->color.B + dB * animation});

      this->pixels.show();
      delay(delayTime / 100);
    }
  }

  void tick() {}
};

#endif
