#include "DimmableSwitch.h"

// Construtor
DimmableSwitch::DimmableSwitch(int DimmableSwitchPin)
    : PIN_InputButton(DimmableSwitchPin), actualState(false), readingValue(false),
      actualState_Last(false), elapse_time(0), holdStartTime(0),
      transition_started(false), isHolding(false), desiredDutyCycle(255), ledDesiredState(false)

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
            actualState_Last = readingValue;

            if (actualState == HIGH) // Botão foi pressionado
            {
                holdStartTime = millis(); // Inicia o contador de "segurar"
                isHolding = false;        // Reseta o estado de "segurando"
                Serial.printf("Button press detected!\n");
                transition_started = false; // Finaliza o debounce
                return true;                // Detecta o evento de pressionamento
            }
            else if (actualState == LOW) // Botão foi solto
            {
                if (isHolding) // Se estava segurando
                {
                    Serial.printf("Button hold released!\n");
                }
                else
                {
                    // Só altera o estado do LED se o botão foi solto antes do tempo necessário para segurar
                    if (millis() - holdStartTime < HOLD_DELAY)
                    {
                        Serial.printf("Button release detected!\n");
                        ledDesiredState = !ledDesiredState;
                    }
                }
                isHolding = false; // Reseta o estado de "segurando"
            }
        }
        transition_started = false;
    }

    // Verifica se o botão está sendo segurado
    if (actualState == HIGH && (millis() - holdStartTime >= HOLD_DELAY))
    {

        if (!isHolding) // Se ainda não foi marcado como segurando
        {
            isHolding = true;
            Serial.printf("Button is being held!\n");
        }
    }
    // Verifica se o botão está pressionado, mas ainda não foi marcado como segurando
    if (isHolding)
    {
        Serial.printf("Hold in progress...!\n"); // Mostra mensagem enquanto está pressionado, mas não segurando
                                                 // Gradually reduce brightness while holding
        if (millis() - elapse_time >= BRIGHTNESS_ADJUST_INTERVAL)
        {
            elapse_time = millis();
            desiredDutyCycle = max(1, desiredDutyCycle - BRIGHTNESS_ADJUST_STEP);
        }
    }

    return false; // Sem mudança de estado
}