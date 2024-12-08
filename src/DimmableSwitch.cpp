#include "DimmableSwitch.h"
/*
OBJETIVO GERAL
usuário da um click no botao, serão 3 situações:
1. click simples: liga/desliga o led
2. click duplo: liga o led no brilho máximo
3. click segurado: diminui o brilho do led

deve-se tomar cuidado para não confundir o click segurado com o click duplo ou simples
ao receber o sinal de click (apos validação do debounce), deve-se primeiro verificar se é um click duplo (o DOUBLE_CLICK_DELAY vale 400)
se não for, deve-se verificar se é um click segurado (o HOLD_DELAY vale 500)
ambos já declarados externamente em ParametersConfig.h
*/
// Construtor
DimmableSwitch::DimmableSwitch(int DimmableSwitchPin)
    : PIN_InputButton(DimmableSwitchPin),
      dutyCycle(255), // Valor inicial de dutyCycle (máximo brilho)
      ledState(false),
      reading(false), // Estado inicial do LED
      lastButtonState(false),
      lastDebounceTime(0),
      buttonState(0),
      clickStartTime(0),
      primeiraVez(false)
{
    pinMode(PIN_InputButton, INPUT);
}

bool DimmableSwitch::detectarMudancaEstadoSwitch()
{
    // Leitura do estado atual do botão
    reading = digitalRead(this->PIN_InputButton);
    // Serial.printf("[detectarMudancaEstadoSwitch] Reading: %d, LastButtonState: %d, ButtonState: %d\n",reading, lastButtonState, buttonState);

    // Detectar mudança no estado do botão
    if (reading != lastButtonState)
    {
        Serial.printf("[detectarMudancaEstadoSwitch] Mudança detectada! Atualizando LastDebounceTime...\n");
        lastDebounceTime = millis(); // Reiniciar o timer de debounce
    }

    // Verificar se o tempo de debounce foi respeitado
    if ((millis() - lastDebounceTime) > DEBOUNCE_TIME)
    {
        // Serial.printf("[detectarMudancaEstadoSwitch] Tempo de debounce passou: %lu ms\n", millis() - lastDebounceTime);
        if (reading != buttonState) // Mudança válida de estado
        {
            buttonState = reading;
            Serial.printf("[detectarMudancaEstadoSwitch] Estado atualizado: ButtonState = %d\n", buttonState);

            if (buttonState == HIGH) // Apenas em estado HIGH
            {
                if (primeiraVez)
                {
                    clickStartTime = millis();
                }
                Serial.printf("[detectarMudancaEstadoSwitch] Click iniciado! clickStartTime = %lu\n", clickStartTime);
                ledState = !ledState;
                Serial.printf("[detectarMudancaEstadoSwitch] LED State atualizado: LEDState = %d\n", ledState);
                // if (logicaDeteccaoClickDuplo())
                // {
                //     Serial.printf("[detectarMudancaEstadoSwitch] Duplo clique detectado!\n");
                //     acaoDuploclick();
                //     return true;
                // }
                // else if (logicaDeteccaoHolding())
                if (logicaDeteccaoHolding())
                {
                    Serial.printf("[detectarMudancaEstadoSwitch] Hold detectado!\n");
                    acaoHolding();
                    Serial.printf(">>>>> retornou true para logicaDeteccaoHolding <<<<<<\n");
                    return true;
                }
                else if (logicaDeteccaoClickSimples())
                {
                    Serial.printf("[detectarMudancaEstadoSwitch] Clique simples detectado!\n");
                    acaoSimplesclick();
                    Serial.printf(">>>>> retornou true para logicaDeteccaoClickSimples <<<<<<\n");
                    return true;
                }
                else
                {
                    Serial.printf("[detectarMudancaEstadoSwitch] Nenhuma ação detectada.\n");
                    Serial.printf(">>>>> retornou true para else <<<<<<\n");
                    return false;
                }
            }
        }
    }

    lastButtonState = reading; // Atualizar estado anterior
    return false;              // Sem mudança
}

bool DimmableSwitch::logicaDeteccaoHolding()
{
    Serial.printf("[logicaDeteccaoHolding] ButtonState = %d, LastButtonState = %d, Tempo decorrido = %lu ms\n",
                  buttonState, lastButtonState, millis() - clickStartTime);

    // Verificar se o botão está pressionado e o tempo de hold foi atingido
    // tem algo errado pois o tempo aqui, mesmo sendo um click simples com menos de 1 segundo
    // está me resultando em um tempo de 5000ms ou mais (no serial monitor)
    if (buttonState == HIGH && lastButtonState == HIGH && ((millis() - clickStartTime) > HOLD_DELAY))
    {
        primeiraVez = true;
        Serial.printf("[logicaDeteccaoHolding] Hold detectado!\n");
        Serial.printf("[logicaDeteccaoHolding] Tempo decorrido: %lu - %lu = %lu ms\n", millis(), clickStartTime, millis() - clickStartTime);
        return true; // Hold detectado
    }
    return false; // Não detectou hold
}

// Lógica para clique duplo
bool DimmableSwitch::logicaDeteccaoClickDuplo()
{
    return true; // apagar
}

// Lógica para clique simples
bool DimmableSwitch::logicaDeteccaoClickSimples()
{
}

// Ajusta o brilho durante "acaoHolding"
void DimmableSwitch::acaoHolding()
{
    Serial.printf("[acaoHolding] Iniciando acaoHolding()...\n");
}

// Seta o brilho máximo no duplo clique
void DimmableSwitch::acaoDuploclick()
{
    Serial.printf("Iniciando acaoDuploclick()...\n");
}

void acaoSimplesclick()
{
    // apos verificar tudo
    // primeiraVez = true;
    Serial.printf("[acaoSimplesclick] Iniciando acaoSimplesclick()...\n");
}
