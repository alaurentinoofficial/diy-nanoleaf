#include "Definitions.h"
#include "Network.h"
#include "Dictionary.h"

#ifndef SerialBus_h
#define SerialBus_h


typedef void(*serialbus_handler_t)(RequestHeader*);

class SerialBus
{
private:
  Dictionary<serialbus_handler_t> events;

public:
  // Compose the command and addresss as a single key to trigger the event
  char* getKey(char cmd, char address)
  {
    String key = String(cmd) + String(address);
    char keyBuffer[key.length() + 1];
    key.toCharArray(keyBuffer, key.length()+1);

    return keyBuffer;
  }

  void clearEvents()
  {
    this->events.clear();
  }

  void registerEvent(char cmd, char address, serialbus_handler_t cb)
  {
    this->events.set(this->getKey(cmd, address), cb);
  }

  void tick()
  {
    if (!SerialBus::available())
      return;

    // Get the request header
    RequestHeader h;
    SerialBus::readHeader(&h);

    Serial.print(h.Address);
    Serial.print(" | ");

    if (this->events[this->getKey(h.Command, h.Address)])
      this->events[this->getKey(h.Command, h.Address)](&h);
  }

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