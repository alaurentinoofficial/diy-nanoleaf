#include "Definitions.h"
#include "Storage.h"

#ifndef ConfigManager_h
#define ConfigManager_h

class ConfigManager
{
private:
    Storage *storage;
    Configs configs;

public:
    ConfigManager()
    {
        storage = new Storage(512);
    }
    bool begin()
    {
        // Read the first bit to check if is already configured
        if (storage->get(0) == 1)
        {
            load();
            return true;
        }

        return false;
    }
    void save()
    {
        this->storage->set(0, 1);
        this->storage->write(1, &this->configs);
    }
    void load()
    {
        this->storage->read(1, &this->configs);
    }
    Configs &get()
    {
        return this->configs;
    }
};

#endif