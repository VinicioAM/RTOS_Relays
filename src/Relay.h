#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay
{
private:
    int pin;    // Relay's Output Pin
    bool state; // Relay's State (on/off)

public:
    // Construtor
    Relay(int relayPin);

    void turnOn();
    void turnOff();
    bool reverseState();
    bool getState();
};

#endif // RELAY_H
