#include "Relay.h"

// Construtor: Configurações iniciais
Relay::Relay(int relayPin) : pin(relayPin), state(false)
{
    //
}

// Liga o relé
bool Relay::reverseState()
{
    state = !state;
    digitalWrite(this->pin, state);
    return state;
}

// Liga o relé
void Relay::turnOn()
{
    state = true;
    digitalWrite(pin, HIGH);
}

// Desliga o relé
void Relay::turnOff()
{
    state = false;
    digitalWrite(pin, LOW);
}

// Retorna o estado atual do relé
bool Relay::getState()
{
    return state;
}