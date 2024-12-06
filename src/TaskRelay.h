#ifndef TASKRELAY_H
#define TASKRELAY_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Relay.h"
#include "Switch.h"

struct TaskRelayParameters
{
    Relay **relays;
    Switch **switches;
    int count;
};

void initializeRelayTask(Relay **relays, Switch **switches);

#endif
