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
  long speed;
public:
  SerialBus(long speed)
  {
    this->speed = speed;
  }

  // Compose the command and addresss as a single key to trigger the event
  char* getKey(char cmd, char address)
  {
    String key = String(cmd) + String(address);
    char* keyBuffer = new char[key.length() + 1];
    key.toCharArray(keyBuffer, key.length()+1);

    return keyBuffer;
  }

  void clearEvents()
  {
    this->events.clear();
  }

  void registerEvent(char cmd, char address, serialbus_handler_t cb)
  {
    char* key = this->getKey(cmd, address);
    this->events.set(key, cb);
    delete key;
  }

  void tick()
  {
    if (!this->available())
      return;

    // Get the request header
    RequestHeader h;
    this->readHeader(&h);

    Serial.print(h.Address);
    Serial.print(" | ");

    char* key = this->getKey(h.Command, h.Address);
    if (this->events[key])
      this->events[key](&h);
    delete key;
  }

  void begin()
  {
    EnlaceLayer::begin(this->speed);
  }

  template <typename T>
  void sendCommand(char commandCode, char address, T *value)
  {
    EnlaceLayer::writeByte(commandCode);
    EnlaceLayer::writeByte(address);
    EnlaceLayer::writeBytes((char *)value, sizeof(T));
    EnlaceLayer::close();
  }

  void readHeader(RequestHeader *h)
  {
    h->Command = EnlaceLayer::readByte();
    h->Address = EnlaceLayer::readByte();
  }

  bool available()
  {
    return EnlaceLayer::available() > 2;
  }

  template <typename T>
  void readPayload(T *payload, char lastChar = '<')
  {
    char b[sizeof(*payload)];
    EnlaceLayer::readBytesUntil(lastChar, b);
    memcpy(payload, b, sizeof(*payload));
  }
};

#endif