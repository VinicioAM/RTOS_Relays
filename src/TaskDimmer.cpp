#include "TaskDimmer.h"
#include "ParametersConfig.h"
#include "Dimmer.h"

void dimmerTask(void *parameters)
{
    struct TaskParameters
    {
        Dimmer **dimmers;
        DimmableSwitch **switches;
        int count;
    };

    TaskParameters *taskParams = (TaskParameters *)parameters;
    Dimmer **dimmers = taskParams->dimmers;
    DimmableSwitch **switches = taskParams->switches;
    int count = taskParams->count;

    while (true)
    {
        for (int i = 0; i < count; i++)
        {
            switches[i]->stateMachine();
            bool aux1 = switches[i]->ledState;
            int aux2 = switches[i]->dutyCycle;
            Serial.printf("Valor atual de ledState = %d, valor atual de dutyCycle = %d\n", aux1, aux2);
            if (switches[i]->ledState)
            {
                dimmers[i]->setBrightness(switches[i]->dutyCycle);
            }
            else
            {
                dimmers[i]->setBrightness(0);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(40)); // Delay de 100ms para evitar processamento desnecessário
    }
}
void initializeDimmerTask(Dimmer *dimmers[], DimmableSwitch *dimmableSwitches[])
{
    static struct TaskParameters
    {
        Dimmer **dimmers;
        DimmableSwitch **switches;
        int count;
    } taskParams;

    taskParams.dimmers = dimmers;
    taskParams.switches = dimmableSwitches;
    taskParams.count = DIMMERS_AND_DIMMABLESWITCHES_COUNT;

    xTaskCreate(
        dimmerTask,    // Função da tarefa
        "Dimmer Task", // Nome da tarefa
        2048,          // Tamanho da pilha
        &taskParams,   // Parâmetros da tarefa
        1,             // Prioridade da tarefa
        NULL           // Handle da tarefa (não usado aqui)
    );
}
