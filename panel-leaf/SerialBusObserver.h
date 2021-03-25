#include "Definitions.h"
#include "Network.h"
#include "Dictionary.h"

#ifndef SerialBusObserver_h
#define SerialBusObserver_h

class SerialBusObserver
{
private:
  Dictionary<void(*)(RequestHeader *)> events;

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

  void registerEvent(char cmd, char address, void (*cb)(RequestHeader *))
  {
    this->events.add(this->getKey(cmd, address), cb);
  }

  void tick()
  {
    if (!TransportLayer::available())
      return;

    // Get the request header
    RequestHeader h;
    TransportLayer::readHeader(&h);

    Serial.print(h.Address);
    Serial.print(" | ");

    if (this->events.get(this->getKey(h.Command, h.Address)))
      this->events.get(this->getKey(h.Command, h.Address))(&h);
  }
};

#endif