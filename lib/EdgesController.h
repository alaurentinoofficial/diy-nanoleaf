#ifndef Edges_h
#define Edges_h

class EdgesController
{
private:
  const char *edgeCodes;
  const int *edgePins;

public:
  EdgesController(const char *edgeCodes, const int *edgePins)
  {
    this->edgeCodes = edgeCodes;
    this->edgePins = edgePins;
  }

  void begin()
  {
    for (unsigned int i = 0; i < sizeof(this->edgePins) / sizeof(char); i++)
      pinMode(this->edgePins[i], INPUT);
  }

  char getTriggedOne()
  {
    for (unsigned int i = 0; i < sizeof(this->edgePins) / sizeof(char); i++)
    {
      if (digitalRead(this->edgePins[i]) == HIGH)
        return this->edgeCodes[i];
    }

    return NONE;
  }

  void setStatus(char code, bool status)
  {
    for (unsigned int i = 0; i < sizeof(this->edgeCodes); i++)
    {
      if (this->edgeCodes[i] == code)
      {
        pinMode(this->edgePins[i], OUTPUT);
        digitalWrite(this->edgePins[i], status ? HIGH : LOW);
        Serial.println(status ? HIGH : LOW);

        if (!status)
          pinMode(this->edgePins[i], INPUT);
        return;
      }
    }
  }
};

#endif
