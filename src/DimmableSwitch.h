#ifndef DIMMABLESWITCH_H
#define DIMMABLESWITCH_H

#include <Arduino.h>
#include "ParametersConfig.h"

class DimmableSwitch
{
private:
    const int PIN_InputButton;
    enum States
    {
        WAITING_FOR_PRESS,
        WAITING_FOR_RELEASE,
        DEFINE_SINGLE_DOUBLE,
        WAITING_BUTTON_RELEASE
    };
    States buttonState;

public:
    // Constructor
    DimmableSwitch(int PIN_InputButton);
    void stateMachine();
    unsigned long inStateAtMs;
};

#endif // DIMMABLESWITCH_H
