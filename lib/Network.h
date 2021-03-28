#include "Definitions.h"

#ifndef Network_h
#define Network_h

class EnlaceLayer
{
public:
  static void begin(long speed)
  {
    Serial.begin(speed);
  }

  static int available()
  {
    return Serial.available();
  }

  static unsigned int writeByte(char value)
  {
    Serial.print(value);
    Serial.print(' ');

    return 1;
  }

  static unsigned int writeBytes(char *payload, unsigned int size_d)
  {
    unsigned int i;
    for (i = 0; i < size_d; i++)
    {
      Serial.print(payload[i]);
      Serial.print(' ');
    }

    return i;
  }

  static void readBytesUntil(char lastChar, char *b)
  {
    Serial.readBytesUntil(lastChar, b, sizeof(b));
  }

  static char readByte()
  {
    return Serial.read();
  }

  static void close()
  {
    Serial.print("<");
    Serial.flush();
    while (Serial.available() > 0)
      Serial.read();
  }
};

#endif
