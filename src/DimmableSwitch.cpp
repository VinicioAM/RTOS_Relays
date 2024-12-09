#include "DimmableSwitch.h"

// Constructor: Initialize variables
DimmableSwitch::DimmableSwitch(int pin)
    : PIN_InputButton(pin),
      buttonState(WAITING_FOR_PRESS),
      inStateAtMs(0)
{
    pinMode(PIN_InputButton, INPUT_PULLUP);
}

void DimmableSwitch::stateMachine()
{
    if (buttonState == WAITING_FOR_PRESS)
    { // 0 = waiting for press
        // wait for button press
        if (millis() - inStateAtMs > DEBOUNCE_TIME)
        {
            if (digitalRead(PIN_InputButton))
            {
                buttonState = WAITING_FOR_RELEASE;
                inStateAtMs = millis();
            }
        }
    }
    else if (buttonState == WAITING_FOR_RELEASE)
    { // 1 = waiting for release
        // wait for stable button release 1
        if (millis() - inStateAtMs > DEBOUNCE_TIME && !digitalRead(PIN_InputButton))
        {
            buttonState = DEFINE_SINGLE_DOUBLE;
            inStateAtMs = millis();
        }
    }
    else if (buttonState == DEFINE_SINGLE_DOUBLE)
    { // 2 = differentiate single or double
        // differentiate between single and double press
        if (millis() - inStateAtMs > DOUBLE_CLICK_DELAY)
        {
            // timeout - is a single press
            Serial.println("single");
            buttonState = WAITING_FOR_PRESS;
            inStateAtMs = millis();
        }
        else if (digitalRead(PIN_InputButton) && (millis() - inStateAtMs > DEBOUNCE_TIME))
        {
            // got second press within timeout - double
            Serial.println("double");
            buttonState = WAITING_BUTTON_RELEASE;
            inStateAtMs = millis();
        }
    }
    else if (buttonState == WAITING_BUTTON_RELEASE)
    { // 3 = wait button release (double)
        // wait for stable button release 2
        // if (millis() - inStateAtMs > DEBOUNCE_TIME && digitalRead(PIN_InputButton)) {
        if (millis() - inStateAtMs > DEBOUNCE_TIME && !digitalRead(PIN_InputButton))
        {
            buttonState = WAITING_FOR_PRESS;
            inStateAtMs = millis();
        }
    }
}