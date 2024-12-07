#ifndef DIMMER_H
#define DIMMER_H

#include <Arduino.h>

class Dimmer
{
private:
    int pin;
    int currentBrightness;
    int memoryBrightness;

public:
    // Construtor
    Dimmer(int relayPin);

    void turnOn();
    void turnOff();
    void reverseState();
    int getBrightness();
    void setBrightness(int brightness);
};

#endif // DIMMER_H
