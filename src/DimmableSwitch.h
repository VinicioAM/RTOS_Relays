#ifndef DIMMABLESWITCH_H
#define DIMMABLESWITCH_H

#include <Arduino.h>
#include "ParametersConfig.h"

class DimmableSwitch
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
    DimmableSwitch(int DimmableSwitchPin);

    // Métodos para controle do Interruptor
    bool readValue(); // Lê o Interruptor
};

#endif // SWITCH_H
