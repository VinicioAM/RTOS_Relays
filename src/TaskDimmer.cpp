#include "TaskDimmer.h"
#include "ParametersConfig.h"

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
            if (switches[i]->dimmableReadValue())
            {
                dimmers[i]->reverseState();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms para evitar processamento desnecessário
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

    xTaskCreate(
        dimmerTask,    // Função da tarefa
        "Dimmer Task", // Nome da tarefa
        2048,          // Tamanho da pilha
        &taskParams,   // Parâmetros da tarefa
        1,             // Prioridade da tarefa
        NULL           // Handle da tarefa (não usado aqui)
    );
}
