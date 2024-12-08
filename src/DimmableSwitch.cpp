#include "DimmableSwitch.h"

// Construtor
DimmableSwitch::DimmableSwitch(int DimmableSwitchPin)
    : PIN_InputButton(DimmableSwitchPin),
      lastButtonState(LOW),
      lastDebounceTime(0),
      buttonState(LOW),
      dutyCycle(255),  // Valor inicial de dutyCycle (máximo brilho)
      ledState(false), // Estado inicial do LED
      isHolding(false)
{
    pinMode(PIN_InputButton, INPUT);
}

// Detecta mudanças de estado do switch com debounce
bool DimmableSwitch::detectarMudancaEstadoSwitch()
{
    int reading = digitalRead(PIN_InputButton);

    // Verifica se houve mudança no estado do botão
    if (reading != lastButtonState)
    {
        lastDebounceTime = millis();
        Serial.printf("[detectarMudancaEstadoSwitch] Mudança detectada, lastDebounceTime: %lu\n", lastDebounceTime);
    }

    // Confirma o estado após o intervalo de debounce
    if ((millis() - lastDebounceTime) > DEBOUNCE_TIME)
    {
        if (reading != buttonState)
        {
            buttonState = reading;
            Serial.printf("[detectarMudancaEstadoSwitch] Novo estado do botão: %d\n", buttonState);

            // Retorna true somente se for uma transição para HIGH
            if (buttonState == HIGH)
            {
                lastButtonState = reading;
                Serial.printf("[detectarMudancaEstadoSwitch] Botão pressionado\n");

                // Chama as funções de lógica baseadas no tipo de clique
                if (logicaDeteccaoHolding())
                    return true;
                if (logicaDeteccaoClickDuplo())
                    return true;
                if (logicaDeteccaoClickSimples())
                    return true;
            }
        }
    }

    lastButtonState = reading;
    return false;
}

// Lógica para acaoHolding
bool DimmableSwitch::logicaDeteccaoHolding()
{
    unsigned long pressStartTime = millis();
    unsigned long pressDuration = 0;

    while (digitalRead(PIN_InputButton) == HIGH)
    {
        pressDuration = millis() - pressStartTime;
        Serial.printf("[logicaDeteccaoHolding] Tempo pressionado: %lu ms\n", pressDuration);

        if (pressDuration > HOLD_DELAY)
        {
            acaoHolding();
            Serial.printf("[logicaDeteccaoHolding] HOLD detectado\n");
            return true;
        }
    }
    return false;
}

// Lógica para clique duplo
bool DimmableSwitch::logicaDeteccaoClickDuplo()
{
    delay(DOUBLE_CLICK_DELAY);
    if (digitalRead(PIN_InputButton) == HIGH)
    {
        acaoDuploclick();
        Serial.printf("[logicaDeteccaoClickDuplo] Duplo clique detectado\n");
        return true;
    }
    return false;
}

// Lógica para clique simples
bool DimmableSwitch::logicaDeteccaoClickSimples()
{
    delay(DOUBLE_CLICK_DELAY);
    if (digitalRead(PIN_InputButton) == HIGH)
        return false; // Clique duplo ou acaoHolding será tratado nas funções específicas

    // Clique simples detectado
    ledState = !ledState;
    Serial.printf("[logicaDeteccaoClickSimples] Clique simples detectado, estado do LED: %d\n", ledState);
    return true;
}

// Ajusta o brilho durante "acaoHolding"
void DimmableSwitch::acaoHolding()
{
    unsigned long lastAdjustTime = millis();
    Serial.printf("[acaoHolding] Iniciando ajuste de brilho, tempo inicial: %lu\n", lastAdjustTime);

    while (digitalRead(PIN_InputButton) == HIGH)
    {
        if (millis() - lastAdjustTime > BRIGHTNESS_ADJUST_INTERVAL)
        {
            dutyCycle = max(1, dutyCycle - BRIGHTNESS_ADJUST_STEP); // Reduz o brilho sem ultrapassar 0
            lastAdjustTime = millis();
            Serial.printf("[acaoHolding] Brightness ajustado: %d, tempo atual: %lu\n", dutyCycle, lastAdjustTime);
        }
    }
}

// Seta o brilho máximo no duplo clique
void DimmableSwitch::acaoDuploclick()
{
    dutyCycle = 255; // Máximo brilho
    Serial.printf("[acaoDuploclick] DutyCycle ajustado para máximo brilho: %d\n", dutyCycle);
}
