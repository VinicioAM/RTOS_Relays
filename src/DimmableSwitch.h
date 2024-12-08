#ifndef DIMMABLESWITCH_H
#define DIMMABLESWITCH_H

#include <Arduino.h>
#include "ParametersConfig.h"

// Definição da estrutura123 como enum
enum estrutura123
{
    SIMPLE_CLICK, // Clique simples
    DOUBLE_CLICK, // Clique duplo
    HOLDING       // Clique segurado (holding)
};

class DimmableSwitch
{
private:
    int PIN_InputButton; // Switch's input pin
    bool actualState;    // Real State of the Pin (after debounce)
    bool readingValue;   // Reading state of the pin (before debounce)
    bool actualState_Last;
    unsigned long elapse_time;
    bool transition_started;
    unsigned long holdStartTime;

public:
    DimmableSwitch(int DimmableSwitchPin);
    bool detectarMudancaEstadoSwitch(); // Lê o Interruptor
    int desiredDutyCycle;
    bool ledDesiredState;

    ////////////////
    //////////////////
    // Se o botão foi liberado antes de `HOLD_DELAY`, verifica o duplo clique
    unsigned long doubleClickStartTime;
    bool isWaitingForDoubleClick;
    bool lastButtonState;
    bool reading;
    unsigned long lastDebounceTime;
    bool buttonState;
    bool isHolding; // Whether the button is being held
    int dutyCycle;
    bool ledState;
    estrutura123 detectarTipoClickUsuario();
    bool logicaClickSimples();
    bool logicaClickDuplo();
    bool logicaHolding();
    void holding();
    void acaoDuploclick();
    bool isHolding;
    unsigned long pressStartTime;
};

#endif // DIMMABLESWITCH_H