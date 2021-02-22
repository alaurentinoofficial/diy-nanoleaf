#include <Arduino.h>
#include <EEPROM.h>

#ifndef Storage_h
#define Storage_h

class Storage
{
public:
    size_t size;

    Storage(size_t size)
    {
        this->size = size;
    };

    void set(int address, uint8_t value)
    {
        EEPROM.write(address, value);
    }

    uint8_t get(int address)
    {
        return EEPROM.read(address);
    }

    void begin()
    {
        EEPROM.begin(this->size);
    }

    template <typename T>
    int write(int ee, const T &value) const
    {
        const byte *p = (const byte *)(const void *)&value;
        unsigned int i;
        for (i = 0; i < sizeof(value); i++)
            EEPROM.write(ee++, *p++);
        return i;
    }

    template <typename T>
    int read(int ee, const T &value) const
    {
        byte *p = (byte *)(void *)&value;
        unsigned int i;
        for (i = 0; i < sizeof(value); i++)
            *p++ = EEPROM.read(ee++);
        return i;
    }
};

#endif