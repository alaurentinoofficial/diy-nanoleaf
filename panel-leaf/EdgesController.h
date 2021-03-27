#ifndef Edges_h
#define Edges_h

#ifdef Arduino_h
class EdgesController
{
private:
  char *edgeCodes;
  int *edgePins;

public:
  EdgesController(char *edgeCodes, int *edgePins)
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

    return NULL;
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

#ifndef Arduino_h
class EdgesController
{
private:
  char *edgeCodes;
  int *edgePins;

public:
  EdgesController(char *edgeCodes, int *edgePins)
  {
    this->edgeCodes = edgeCodes;
    this->edgePins = edgePins;
  }

  void begin()
  {
  }

  char getTriggedOne()
  {
    // Return first
    for (unsigned int i = 0; i < sizeof(this->edgePins) / sizeof(char); i++)
      return this->edgeCodes[i];
  }

  void setStatus(char code, bool status)
  {
    cout << "Write edge " << code << (status ? "High" : "Low") << endl; 
  }
};
#endif

#endif
