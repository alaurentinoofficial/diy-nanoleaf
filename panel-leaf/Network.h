#include "Definitions.h"

#ifndef Network_h
#define Network_h

#ifdef Arduino_h
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

  static unsigned int writeBytes(char *payload, int size_d)
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

#ifndef Arduino_h
#import <iostream>

using namespace std;

class EnlaceLayer
{
public:
  static void begin(long speed)
  {
    cout << "Set Serial Rate> " << speed << "hz" << endl;
  }

  static int available()
  {
    return Serial.available();
  }

  static unsigned int writeByte(char value)
  {
    cout << value << " ";

    return 1;
  }

  static unsigned int writeBytes(char *payload, int size_d)
  {
    unsigned int i;
    for (i = 0; i < size_d; i++)
    {
      writeByte(payload[i]);
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
    writeByte("<");
  }
};
#endif

class TransportLayer
{
public:
  static void begin(long speed)
  {
    EnlaceLayer::begin(speed);
  }

  template <typename T>
  static void sendCommand(char commandCode, char address, T *value)
  {
    EnlaceLayer::writeByte(commandCode);
    EnlaceLayer::writeByte(address);
    EnlaceLayer::writeBytes((char *)value, sizeof(T));
    EnlaceLayer::close();
  }

  static void readHeader(RequestHeader *h)
  {
    h->Command = EnlaceLayer::readByte();
    h->Address = EnlaceLayer::readByte();
  }

  static bool available()
  {
    return EnlaceLayer::available() > 2;
  }

  template <typename T>
  static void readPayload(T *payload, char lastChar = '<')
  {
    char b[sizeof(*payload)];
    EnlaceLayer::readBytesUntil(lastChar, b);
    memcpy(payload, b, sizeof(*payload));
  }
};

#endif
