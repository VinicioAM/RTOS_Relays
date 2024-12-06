#include "Switch.h"

// Construtor: inicializa o pino e define o estado inicial
Switch::Switch(int SwitchPin)
    : PIN_InputButton(SwitchPin), actualState(false), readingValue(false),
      actualState_Last(false), elapse_time(0), transition_started(false)
{
    pinMode(PIN_InputButton, INPUT); // Configura o pino como entrada
}

// RLê o Interruptor
bool Switch::readValue()
{
    readingValue = digitalRead(this->PIN_InputButton);

    // if (InputPin_Value != relay.getOutputState() && !transition_started)
    if (readingValue != actualState_Last && !transition_started)
    {
        // A switch change is detected, so start the debounce cycle
        transition_started = true;
        elapse_time = millis(); // Start the debounce timer
    }

    if (transition_started)
    {
        // We are in the switch transition cycle, check if debounce period has elapsed
        if (millis() - elapse_time >= DEBOUNCE_TIME)
        {
            // Debounce period elapsed, assume the switch has settled
            // relay.setOutputState(!relay.getOutputState());
            transition_started = false; // End the transition cycle
            actualState = readingValue;
        }
    }
    actualState_Last = readingValue;
    return actualState;
}