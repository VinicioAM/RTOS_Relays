#include "Dimmer.h"
#include "ParametersConfig.h"

// Construtor: Initial Config
Dimmer::Dimmer(int DimmerPin) : pin(DimmerPin)
{
    //
}

void Dimmer::reverseState()
{
    if (currentBrightness == 255)
    {
        turnOff();
    }
    else
    {
        turnOn();
    }
}

void Dimmer::turnOn()
{
    setBrightness(255); // Configura brilho para 0
}

void Dimmer::turnOff()
{
    setBrightness(0); // Configura brilho para 0
}

int Dimmer::getBrightness()
{
    return currentBrightness;
}

void Dimmer::setBrightness(int brightness)
{
    currentBrightness = brightness;
    ledcWrite(PWM_CHANNEL, brightness); // Atualiza o valor PWM
}