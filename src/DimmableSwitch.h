#ifndef DIMMABLESWITCH_H
#define DIMMABLESWITCH_H

#include <Arduino.h>
#include "ParametersConfig.h"

class DimmableSwitch
{
private:
    int PIN_InputButton; // Switch's input pin
    bool actualState;    // Real State of the Pin (after debounce)
    bool readingValue;   // Reading state of the pin (before debounce)
    bool actualState_Last;
    unsigned long elapse_time;
    bool transition_started;
    bool isHolding; // Whether the button is being held
    unsigned long holdStartTime;

public:
    DimmableSwitch(int DimmableSwitchPin);
    bool dimmableReadValue(); // Lê o Interruptor
    int desiredDutyCycle;
    bool ledDesiredState;
};

#endif // DIMMABLESWITCH_H