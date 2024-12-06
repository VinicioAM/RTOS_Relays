#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay
{
private:
    int pin;    // Pino do relé
    bool state; // Estado do relé (ligado/desligado)

public:
    // Construtor
    Relay(int relayPin);

    // Métodos para controle do relé
    void turnOn();       // Liga o relé
    void turnOff();      // Desliga o relé
    bool reverseState(); // Inverte o estado atual do relé
    bool getState();     // Retorna o estado atual do relé
};

#endif // RELAY_H
