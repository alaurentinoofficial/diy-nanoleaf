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
    ConfigManager() {
        storage = new Storage(512);
    }
    bool begin() {
        if(storage->get(0) == 1) {
            load();
            return true;
        }

        return false;
    }
    void save() {
        this->storage->write(1, &this->configs);
    }
    void load() {
        this->storage->read(1, &this->configs);
    }
    Configs &get()
    {
        return this->configs;
    }
};

#endif