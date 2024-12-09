#include "DimmableSwitch.h"

// Constructor: Initialize variables
DimmableSwitch::DimmableSwitch(int pin)
    : PIN_InputButton(pin),
      buttonState(WAITING_FOR_PRESS),
      inStateAtMs(0),
      ledState(false),
      dutyCycle(255)
{
    pinMode(PIN_InputButton, INPUT_PULLUP);
}

void DimmableSwitch::stateMachine()
{
    if (buttonState == WAITING_FOR_PRESS)
    { // 0 = waiting for press
        Serial.printf("State: WAITING_FOR_PRESS\n");
        // wait for button press
        if (millis() - inStateAtMs > DEBOUNCE_TIME)
        {
            if (digitalRead(PIN_InputButton))
            {
                Serial.printf("Button pressed, transitioning to WAITING_FOR_RELEASE\n");
                buttonState = WAITING_FOR_RELEASE;
                inStateAtMs = millis();
            }
        }
    }
    else if (buttonState == WAITING_FOR_RELEASE)
    { // 1 = waiting for release
        Serial.printf("State: WAITING_FOR_RELEASE\n");
        // wait for stable button release
        if (millis() - inStateAtMs > HOLD_DELAY && digitalRead(PIN_InputButton))
        {
            // Button held long enough to detect hold
            Serial.printf("Button held long enough, transitioning to HOLDING\n");
            buttonState = HOLDING;
            inStateAtMs = millis();
        }
        else if (millis() - inStateAtMs > DEBOUNCE_TIME && !digitalRead(PIN_InputButton))
        {
            // Button released without holding
            Serial.printf("Button released, transitioning to DEFINE_SINGLE_DOUBLE\n");
            buttonState = DEFINE_SINGLE_DOUBLE;
            inStateAtMs = millis();
        }
    }
    else if (buttonState == HOLDING)
    { // 4 = holding
        Serial.printf("State: HOLDING\n");
        // Wait for the button to be released after holding
        holdingAction();
        if (millis() - inStateAtMs > DEBOUNCE_TIME && !digitalRead(PIN_InputButton))
        {
            Serial.printf("Button released after holding, transitioning to WAITING_FOR_PRESS\n");
            buttonState = WAITING_FOR_PRESS;
            inStateAtMs = millis();
        }
    }
    else if (buttonState == DEFINE_SINGLE_DOUBLE)
    { // 2 = differentiate single or double
        Serial.printf("State: DEFINE_SINGLE_DOUBLE\n");
        // differentiate between single and double press
        if (millis() - inStateAtMs > DOUBLE_CLICK_DELAY)
        {
            // timeout - is a single press
            Serial.printf("Single press detected\n");
            singlePressAction();
            buttonState = WAITING_FOR_PRESS;
            inStateAtMs = millis();
        }
        else if (digitalRead(PIN_InputButton) && (millis() - inStateAtMs > DEBOUNCE_TIME))
        {
            // got second press within timeout - double
            Serial.printf("Double press detected, transitioning to WAITING_BUTTON_RELEASE\n");
            doublePressAction();
            buttonState = WAITING_BUTTON_RELEASE;
            inStateAtMs = millis();
        }
    }
    else if (buttonState == WAITING_BUTTON_RELEASE)
    { // 3 = wait button release (double)
        Serial.printf("State: WAITING_BUTTON_RELEASE\n");
        // wait for stable button release 2
        if (millis() - inStateAtMs > DEBOUNCE_TIME && !digitalRead(PIN_InputButton))
        {
            Serial.printf("Button released after double press, transitioning to WAITING_FOR_PRESS\n");
            buttonState = WAITING_FOR_PRESS;
            inStateAtMs = millis();
        }
    }
}

void DimmableSwitch::holdingAction()
{
    Serial.printf("Holding action initiated\n");
    ledState = true;
    dutyCycle = max(dutyCycle - BRIGHTNESS_ADJUST_STEP, 1);
}

void DimmableSwitch::singlePressAction()
{
    Serial.printf("Single press action initiated\n");
    ledState = !ledState;
}

void DimmableSwitch::doublePressAction()
{
    Serial.printf("Double press action initiated\n");
    ledState = true;
    dutyCycle = 255;
}