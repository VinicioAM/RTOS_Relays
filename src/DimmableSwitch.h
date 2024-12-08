#ifndef DIMMABLESWITCH_H
#define DIMMABLESWITCH_H

#include <Arduino.h>
#include "ParametersConfig.h"

// Definição da TiposDeClick como enum
enum TiposDeClick
{
    SIMPLE_CLICK, // Clique simples
    DOUBLE_CLICK, // Clique duplo
    HOLDING       // Clique segurado (acaoHolding)
};

class DimmableSwitch
{
private:
    int PIN_InputButton; // Switch's input pin
    bool lastButtonState;
    bool reading;
    unsigned long lastDebounceTime;
    bool buttonState;
    bool isHolding; // Whether the button is being held
    TiposDeClick detectarTipoClickUsuario();
    bool logicaDeteccaoClickSimples();
    bool logicaDeteccaoClickDuplo();
    bool logicaDeteccaoHolding();
    void acaoHolding();
    void acaoDuploclick();

public:
    DimmableSwitch(int DimmableSwitchPin);
    bool detectarMudancaEstadoSwitch(); // Lê o Interruptor
    bool ledState;
    int dutyCycle;
};

#endif // DIMMABLESWITCH_H