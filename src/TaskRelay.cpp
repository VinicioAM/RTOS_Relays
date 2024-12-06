#include "TaskRelay.h"
#include "ParametersConfig.h"
#include "TaskMQTT.h"

static QueueHandle_t Queue_Relay = NULL;
bool lastSwitchStates[RELAYS_AND_SWITCHES_COUNT];
static bool lastRelayStates[RELAYS_AND_SWITCHES_COUNT];

// Generic structure for queue messages, which can come from MQTT or SWITCH
typedef struct
{
    int index;     // Relay index
    bool state;    // Desired state (true = on, false = off)
    bool fromMQTT; // true if the message came from MQTT, false if from SWITCH
} TMessage_Relay;

void setRelayStateFromMQTT(int relayIndex, bool desiredState)
{
    // Verify the validity of the index
    if (relayIndex < 0 || relayIndex >= RELAYS_AND_SWITCHES_COUNT)
    {
        Serial.printf("Invalid relay index when trying to send to queue: %d\n", relayIndex);
        return;
    }

    TMessage_Relay msg;
    msg.index = relayIndex;
    msg.state = desiredState;
    msg.fromMQTT = true;

    if (uxQueueSpacesAvailable(Queue_Relay) == 0)
    {
        Serial.println("Queue_Relay is full! Message discarded.");
        return;
    }

    if (xQueueSend(Queue_Relay, &msg, 10 / portTICK_PERIOD_MS) != pdPASS)
    {
        Serial.println("Failed to send message to Queue_Relay.");
    }
}

// Main relay task
void relayTask(void *parameter)
{
    TaskRelayParameters *params = (TaskRelayParameters *)parameter;
    Relay **relays = params->relays;
    Switch **switches = params->switches;

    for (int i = 0; i < RELAYS_AND_SWITCHES_COUNT; i++)
    {
        lastSwitchStates[i] = switches[i]->readValue();
    }

    TMessage_Relay receivedMsg;

    while (true)
    {
        // First, handle any message from the queue
        if (xQueueReceive(Queue_Relay, &receivedMsg, 0) == pdTRUE)
        {
            // Check if the relay index is within the limit
            if (receivedMsg.index < 0 || receivedMsg.index >= RELAYS_AND_SWITCHES_COUNT)
            {
                Serial.printf("Invalid relay index received: %d. Message ignored.\n", receivedMsg.index);
                // If the index is invalid, skip further processing of this message
                continue;
            }

            // Set the relay state based on the received message
            if (receivedMsg.state)
            {
                relays[receivedMsg.index]->turnOn();
            }
            else
            {
                relays[receivedMsg.index]->turnOff();
            }
            // Publish the new state to MQTT
            publishToMQTT(receivedMsg.index, receivedMsg.state);
        }

        // Next, monitor the physical switches
        for (int i = 0; i < RELAYS_AND_SWITCHES_COUNT; ++i)
        {
            bool currentSwitchState = switches[i]->readValue();
            if (currentSwitchState != lastSwitchStates[i])
            {
                // A switch state change occurred
                lastSwitchStates[i] = currentSwitchState;

                // Read the current state of the relay
                bool currentRelayState = relays[i]->getState(); // Example method, adjust as per your implementation
                bool newRelayState = !currentRelayState;        // Toggle the current state

                // Set the relay to the new state
                if (newRelayState)
                {
                    relays[i]->turnOn();
                }
                else
                {
                    relays[i]->turnOff();
                }

                // Publish the new state to MQTT
                publishToMQTT(i, currentSwitchState);
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Task initialization
void initializeRelayTask(Relay **relays, Switch **switches)
{
    Queue_Relay = xQueueCreate(20, sizeof(TMessage_Relay));

    for (int i = 0; i < RELAYS_AND_SWITCHES_COUNT; i++)
    {
        lastRelayStates[i] = false; // or retrieve the initial relay state if necessary
    }

    static TaskRelayParameters params;
    params.relays = relays;
    params.switches = switches;
    params.count = RELAYS_AND_SWITCHES_COUNT;

    xTaskCreatePinnedToCore(
        relayTask,   // Task function
        "RelayTask", // Task name
        10000,       // Stack size
        &params,     // Task parameter
        1,           // Priority
        NULL,        // Task handle
        1            // Core where the task will run (ESP32: 0 or 1)
    );
}
