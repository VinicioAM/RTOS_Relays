#include "ParametersConfig.h"
#include "TaskWiFi.h"
#include "TaskMQTT.h"
#include "TaskRelay.h"

// Queue for MQTT messages
static QueueHandle_t xQueue_MQTT = NULL;

// MQTT client
static WiFiClient wifiClient;
static PubSubClient MQTT(wifiClient);

// Internal function prototypes
static void mqttTask(void *pvParameters);
static void initMQTT();
static void connectMQTT();
static void mqttCallback(char *topic, byte *payload, unsigned int length);

void initializeMQTTTask()
{
    // Creates a queue for sending messages
    xQueue_MQTT = xQueueCreate(20, sizeof(TMessage_MQTT));
    if (xQueue_MQTT == NULL)
    {
        Serial.println("Failed to create MQTT queue!");
        // Handle error if necessary
    }

    xTaskCreatePinnedToCore(
        mqttTask,   // task function
        "MQTTTask", // task name
        4096,       // stack size
        NULL,       // parameter
        5,          // priority
        NULL,       // handle
        1           // ESP32 core
    );
}

// Function to insert a message into the MQTT queue
void publishToMQTT(int relayIndex, bool state)
{
    if (xQueue_MQTT == NULL)
        return; // if the queue does not exist
    TMessage_MQTT msg;
    msg.relayIndex = relayIndex;
    msg.state = state;
    xQueueSend(xQueue_MQTT, &msg, 0);
}

// Internal function to initialize MQTT
static void initMQTT()
{
    MQTT.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTT.setCallback(mqttCallback);
}

// Internal function to connect to the MQTT broker
static void connectMQTT()
{
    while (!MQTT.connected())
    {
        Serial.println("Connecting to MQTT broker...");
        if (MQTT.connect("ESP32_Client"))
        {
            Serial.println("Connected to MQTT broker!");
            MQTT.subscribe(MQTT_TOPIC_IN);
            Serial.printf("Subscribed to topic: %s\n", MQTT_TOPIC_IN);
        }
        else
        {
            Serial.print("MQTT connection failed. Error code: ");
            Serial.println(MQTT.state());
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
}

// Callback to receive messages from the MQTT broker
static void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.printf("Message received on topic: %s\n", topic);
    payload[length] = '\0'; // Null-terminate the received string
    Serial.printf("Payload: %s\n", (char *)payload);

    // Exemplo de decodificação simples do payload MQTT "xy":
    // x = índice do relay (char), y = estado '0' ou '1'
    char relayChar = payload[0];
    char stateChar = payload[1];

    int relayIndex = relayChar - '0';
    bool desiredState = (stateChar == '1');

    // Chamar a função para enviar o evento ao relayTask
    Serial.printf("msg recebida -> %s\n", (char *)payload);
    Serial.printf("relayChar -> %c\n", relayChar);
    Serial.printf("stateChar -> %c\n", stateChar);
    Serial.printf("relayIndex -> %d\n", relayIndex);
    Serial.printf("desiredState -> %d\n", desiredState);
    setRelayStateFromMQTT(relayIndex, desiredState);
}

// MQTT task that maintains WiFi connection, MQTT connection, and publishes messages from the queue
static void mqttTask(void *pvParameters)
{
    initMQTT();

    for (;;)
    {
        // Check WiFi connection
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Wi-Fi disconnected. Trying to reconnect...");
            reconnectWiFi();
        }
        else
        {
            // Connect to MQTT broker if necessary
            if (!MQTT.connected())
            {
                connectMQTT();
            }
            // Check queue to publish messages
            if (MQTT.connected() && xQueue_MQTT != NULL)
            {
                TMessage_MQTT msg;
                // Try to receive a message from the queue (non-blocking)
                if (xQueueReceive(xQueue_MQTT, &msg, 10 / portTICK_PERIOD_MS) == pdPASS)
                {
                    // Publish the received message
                    char message[50];
                    snprintf(message, sizeof(message), "%d%d", msg.relayIndex, msg.state ? 1 : 0);
                    MQTT.publish(MQTT_TOPIC_OUT, message);
                    Serial.printf("Published on topic %s : %s\n", MQTT_TOPIC_OUT, message);
                }
            }
            MQTT.loop();
        }
        // Wait interval to avoid excessive CPU usage
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}