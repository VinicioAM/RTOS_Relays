#include "ParametersConfig.h"
#include "TaskWiFi.h"
#include "TaskMQTT.h"
#include "TaskRelay.h"

#include <AsyncMqttClient.h>

// Queue for MQTT messages
static QueueHandle_t xQueue_MQTT = NULL;

// Async MQTT Client
static AsyncMqttClient mqttClient;

// Internal function prototypes
static void mqttTask(void *pvParameters);
static void initMQTT();
static void connectMQTT();

// AsyncMqttClient Callbacks
static void onMqttConnect(bool sessionPresent);
static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
static void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties,
                          size_t len, size_t index, size_t total);

void printMQTTStatus()
{
    if (mqttClient.connected())
    {
        Serial.println("MQTT Status: Connected");
        Serial.printf("Subscribed Topic: %s\n", MQTT_TOPIC_IN);
    }
    else
    {
        Serial.println("MQTT Status: Disconnected");
    }
}

void initializeMQTTTask()
{
    // Create the queue for MQTT messages
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

// Callback for receiving messages from the MQTT broker
static void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties,
                          size_t len, size_t index, size_t total)
{
    // Ensure the payload string is null-terminated
    char tempPayload[128];
    if (len < sizeof(tempPayload))
    {
        memcpy(tempPayload, payload, len);
        tempPayload[len] = '\0';

        Serial.printf("RECEIVED -> Topic = %s, Payload = %s\n", topic, tempPayload);

        // Simple decoding example of the payload "xy":
        // x = relay index (char), y = state '0' or '1'
        char relayChar = tempPayload[0];
        char stateChar = tempPayload[1];

        int relayIndex = relayChar - '0';
        bool desiredState = (stateChar == '1');

        // Call the function to send the event to relayTask
        setRelayStateFromMQTT(relayIndex, desiredState);
    }
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
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
}

// Function called when connected to the MQTT broker
static void onMqttConnect(bool sessionPresent)
{
    Serial.println("Connected to MQTT broker!");
    uint16_t packetIdSub = mqttClient.subscribe(MQTT_TOPIC_IN, MQTT_QOS);
    Serial.printf("Subscribed to topic: %s, PacketId: %d\n", MQTT_TOPIC_IN, packetIdSub);
}

// Function called when disconnected from the MQTT broker
static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.println("Disconnected from MQTT. Trying to reconnect...");
}

// Function to connect to the MQTT broker with retries
static void connectMQTT()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connecting to MQTT broker...");
        mqttClient.connect();
    }
}

// MQTT task that maintains WiFi connection, MQTT connection and publishes messages from the queue
static void mqttTask(void *pvParameters)
{
    initMQTT();

    for (;;)
    {
        // Check Wi-Fi connection
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Wi-Fi disconnected. Trying to reconnect...");
            // reconnectWiFi(); //managed by WiFi Task
        }
        else
        {
            // Connect to the MQTT broker if necessary
            if (!mqttClient.connected())
            {
                connectMQTT();
            }

            // If connected to MQTT and the queue exists
            if (mqttClient.connected() && xQueue_MQTT != NULL)
            {
                TMessage_MQTT msg;
                // Attempt to receive a message from the queue (non-blocking)
                if (xQueueReceive(xQueue_MQTT, &msg, 10 / portTICK_PERIOD_MS) == pdPASS)
                {
                    // Publish the received message
                    char message[50];
                    snprintf(message, sizeof(message), "%d%d", msg.relayIndex, msg.state ? 1 : 0);
                    mqttClient.publish(MQTT_TOPIC_OUT, MQTT_QOS, false, message);
                    Serial.printf("SENT -> Topic = %s, Payload = %s\n", MQTT_TOPIC_OUT, message);
                }
            }
        }
        // Interval to avoid excessive CPU usage
        vTaskDelay(TIMESTEP_MQTT / portTICK_PERIOD_MS);
    }
}
