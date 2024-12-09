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
        WAITING_BUTTON_RELEASE,
        HOLDING
    };
    States buttonState;
    void holdingAction();
    void singlePressAction();
    void doublePressAction();
    unsigned long inStateAtMs;

public:
    // Constructor
    DimmableSwitch(int PIN_InputButton);
    void stateMachine();
    int dutyCycle;
    bool ledState;
};

#endif // DIMMABLESWITCH_H
