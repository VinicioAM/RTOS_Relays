#include "DimmableSwitch.h"

// Construtor
DimmableSwitch::DimmableSwitch(int DimmableSwitchPin)
    : PIN_InputButton(DimmableSwitchPin), actualState(false), readingValue(false),
      actualState_Last(false), elapse_time(0), transition_started(false)
{
    pinMode(PIN_InputButton, INPUT); // Configura o pino como entrada
}

bool DimmableSwitch::dimmableReadValue()
{
    readingValue = digitalRead(this->PIN_InputButton);

    if (readingValue != actualState_Last && !transition_started)
    {
        // A DimmableSwitch change is detected, so start the debounce cycle
        transition_started = true;
        elapse_time = millis(); // Start the debounce timer
    }

    if (transition_started)
    {
        // We are in the DimmableSwitch transition cycle, check if debounce period has elapsed
        if (millis() - elapse_time >= DEBOUNCE_TIME)
        {
            // Debounce period elapsed, assume the DimmableSwitch has settled
            // relay.setOutputState(!relay.getOutputState());
            transition_started = false; // End the transition cycle
            actualState = readingValue;
        }
    }
    actualState_Last = readingValue;
    return actualState;
}