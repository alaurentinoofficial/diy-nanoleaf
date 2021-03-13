#include <Arduino.h>

#ifndef Definitions_h
#define Definitions_h

struct RGB
{
  byte r;
  byte g;
  byte b;
};

enum Modes {
    Static = 0,
    Rain = 1,
    Rainbow = 3
};

struct Configs {
    char* wifiSSID;
    char* wifiPassword;
    char* mDnsName;

    int qtyLedsPerLeaf;
    int qtyLeafs;
    Modes mode;
    int speed;
    RGB primaryColor;
    RGB secodaryColor;

    int getQtyLeds() {
        return this->qtyLeafs * this->qtyLedsPerLeaf;
    }
};

#endif