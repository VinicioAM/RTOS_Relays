#include "Relay.h"

// Construtor: Initial Config
Relay::Relay(int relayPin) : pin(relayPin), state(false)
{
    //
}

bool Relay::reverseState()
{
    state = !state;
    digitalWrite(this->pin, state);
    return state;
}

void Relay::turnOn()
{
    state = true;
    digitalWrite(pin, HIGH);
}

void Relay::turnOff()
{
    state = false;
    digitalWrite(pin, LOW);
}

bool Relay::getState()
{
    return state;
}