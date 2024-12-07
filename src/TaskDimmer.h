#ifndef TASK_DIMMER_H
#define TASK_DIMMER_H

#include <Arduino.h>
#include "Dimmer.h"
#include "DimmableSwitch.h"

void initializeDimmerTask(Dimmer *dimmers[], DimmableSwitch *dimmableSwitches[]);

#endif // TASK_DIMMER_H
