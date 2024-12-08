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
        transition_started = true;
        elapse_time = millis();
    }

    if (transition_started && millis() - elapse_time >= DEBOUNCE_TIME)
    {
        if (readingValue != actualState)
        {
            actualState = readingValue; // Atualiza o estado real
            transition_started = false; // Finaliza o debounce
            actualState_Last = readingValue;

            if (actualState == HIGH) // Botão foi pressionado
            {
                Serial.printf("Button press detected!\n");
                return true; // Detecta o evento de pressionamento
            }
        }
        transition_started = false;
    }
    return false; // Sem mudança de estado
}