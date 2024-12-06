#include "TaskRelay.h"
#include "ParametersConfig.h"
#include "TaskMQTT.h"

void relayTask(void *parameter)
{
    TaskRelayParameters *params = (TaskRelayParameters *)parameter;
    Relay **relays = params->relays;
    Switch **switches = params->switches;
    // bool *lastStates = new bool[count];
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
        relayTask,   // Função da task
        "RelayTask", // Nome da task
        1000,        // Tamanho da stack
        &params,     // Parâmetro da task
        1,           // Prioridade
        NULL,        // Handle da task
        1            // Núcleo onde rodará a task (0 ou 1 no ESP32)
    );
}
