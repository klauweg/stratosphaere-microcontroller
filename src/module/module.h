#include <Arduino.h>

#ifndef MODULE_H
#define MODULE_H


class Module {
    protected:
        uint8_t status = 1;
    public:
        virtual void configure() = 0;
        virtual void tick() = 0;
        uint8_t getStatus() {return this->status;};
};

#endif
