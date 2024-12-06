#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>
#include "ParametersConfig.h"

class Switch
{
private:
    int PIN_InputButton; // Switch's input pin
    bool actualState;    // Real State of the Pin (after debounce)
    bool readingValue;   // Reading state of the pin (before debounce)
    bool actualState_Last;
    unsigned long elapse_time;
    bool transition_started;

public:
    // Construtor
    Switch(int SwitchPin);

    bool readValue(); // Lê o Interruptor
};

#endif // SWITCH_H
