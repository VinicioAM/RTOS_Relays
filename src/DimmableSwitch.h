#ifndef DIMMABLESWITCH_H
#define DIMMABLESWITCH_H

#include <Arduino.h>
#include "ParametersConfig.h"

class DimmableSwitch
{
private:
    int PIN_InputButton; // Switch's input pin
    bool logicaDeteccaoClickSimples();
    bool logicaDeteccaoClickDuplo();
    bool logicaDeteccaoHolding();
    void acaoHolding();
    void acaoDuploclick();
    void acaoSimplesclick();

    bool reading;
    bool buttonState;
    bool lastButtonState;
    unsigned long lastDebounceTime;
    unsigned long clickStartTime;
    bool primeiraVez;

public:
    DimmableSwitch(int DimmableSwitchPin);
    bool detectarMudancaEstadoSwitch(); // Lê o Interruptor
    bool ledState;
    int dutyCycle;
};

#endif // DIMMABLESWITCH_H