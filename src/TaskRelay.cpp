#include "TaskRelay.h"
#include "ParametersConfig.h"
#include "TaskMQTT.h"

void relayTask(void *parameter)
{
    TaskRelayParameters *params = (TaskRelayParameters *)parameter;
    Relay **relays = params->relays;
    Switch **switches = params->switches;
    bool lastStates[RELAYS_AND_SWITCHES_COUNT];

    while (true)
    {
        for (int i = 0; i < RELAYS_AND_SWITCHES_COUNT; ++i)
        {
            if (switches[i]->readValue() != lastStates[i])
            {
                lastStates[i] = relays[i]->reverseState();
                publishToMQTT(i, lastStates[i]);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void initializeRelayTask(Relay **relays, Switch **switches)
{
    static TaskRelayParameters params;
    params.relays = relays;
    params.switches = switches;
    params.count = RELAYS_AND_SWITCHES_COUNT;

    xTaskCreatePinnedToCore(
        relayTask,   // Task function
        "RelayTask", // Task name
        1000,        // Stack size
        &params,     // Task parameter
        1,           // Priority
        NULL,        // Task handle
        1            // Core where the task will run (0 or 1 on ESP32)
    );
}
