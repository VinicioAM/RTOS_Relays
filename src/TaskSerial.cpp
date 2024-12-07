#include "TaskSerial.h"

void serialMonitorTask(void *param)
{
    Serial.begin(115200);
    Serial.println("Serial Monitor Task initialized.");

    String receivedData;

    while (true)
    {
        if (Serial.available())
        {
            receivedData = Serial.readStringUntil('\n');
            receivedData.trim(); // Remove espaços e quebras de linha

            if (receivedData.equalsIgnoreCase("mqtt"))
            {
                Serial.println("MQTT status requested.");
                printMQTTStatus();
            }
            else if (receivedData.equalsIgnoreCase("wifi"))
            {
                Serial.println("Wi-Fi status requested.");
                printWIFIStatus();
            }
            else
            {
                Serial.println("Unknown command received: " + receivedData);
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS); // Delay de 100ms para evitar loop constante
    }
}

void initializeSerialMonitorTask()
{
    xTaskCreatePinnedToCore(
        serialMonitorTask,   // Função da tarefa
        "SerialMonitorTask", // Nome da tarefa
        4096,                // Tamanho da stack
        NULL,                // Parâmetro
        1,                   // Prioridade
        NULL,                // Handle
        1                    // Núcleo em que a tarefa será executada
    );
}
