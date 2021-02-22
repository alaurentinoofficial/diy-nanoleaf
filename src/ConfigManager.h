#include "Definitions.h"

#ifndef ConfigManager_h
#define ConfigManager_h

class ConfigManager
{
private:
    Configs configs;

public:
    ConfigManager() {}
    bool begin() { return false; }
    void save() {}
    void load() {}
    Configs &get()
    {
        return this->configs;
    }
};

#endif