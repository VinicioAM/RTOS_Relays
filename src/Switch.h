#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>
#include "ParametersConfig.h"

class Switch
{
private:
    int PIN_InputButton; // Pino do Interruptor
    bool actualState;    // Estado Real do Interruptor
    bool readingValue;   // Estado Lido no Pino
    bool actualState_Last;
    unsigned long elapse_time;
    bool transition_started;

public:
    // Construtor
    Switch(int SwitchPin);

    // Métodos para controle do Interruptor
    bool readValue(); // Lê o Interruptor
};

#endif // SWITCH_H
