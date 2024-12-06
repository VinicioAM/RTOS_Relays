#include "TaskMQTT.h"
#include "ParametersConfig.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

static void mqttCallback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message: ");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
    }
    Serial.println();
}

void reconnectToMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.println("Connecting to MQTT broker...");
        if (mqttClient.connect("ESP32Client"))
        {
            Serial.println("Connected to MQTT broker.");
            mqttClient.subscribe(MQTT_TOPIC);
        }
        else
        {
            Serial.print("Failed to connect. State: ");
            Serial.println(mqttClient.state());
            Serial.println("Retrying in 5 seconds...");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }
}

void mqttTask(void *param)
{
    mqttClient.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
    mqttClient.setCallback(mqttCallback);
    while (true)
    {
        if (!mqttClient.connected())
        {
            reconnectToMQTT();
        }
        mqttClient.loop();
        vTaskDelay(100 / portTICK_PERIOD_MS); // Loop every 100ms
    }
}

void initializeMQTTTask()
{
    xTaskCreatePinnedToCore(
        mqttTask,   // Função da task
        "MQTTTask", // Nome da task
        10000,      // Tamanho da stack
        NULL,       // Parâmetros
        1,          // Prioridade
        NULL,       // Handle
        1           // Core para rodar a task
    );
}

void publishToMQTT(int relayIndex, bool state)
{
    Serial.println("antes do if");

    if (!mqttClient.connected())
    {
        Serial.println("dentro do if");

        Serial.println("MQTT not connected. Unable to publish message.");
        return;
    }
    Serial.println("apos do if");
    Serial.println("a");
    mqttClient.beginPublish(MQTT_TOPIC, 50, false); // Tamanho máximo esperado (ajustável)
    mqttClient.print("Relay ");
    mqttClient.print(relayIndex);
    mqttClient.print(", Status = ");
    mqttClient.print(state);
    Serial.println("b");
    mqttClient.endPublish();
}
