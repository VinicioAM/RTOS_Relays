#ifndef TASK_MQTT_H
#define TASK_MQTT_H

#include <Arduino.h>
#include <WiFi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <AsyncMqttClient.h>

typedef struct
{
    int relayIndex;
    bool state;
} TMessage_MQTT;

void initializeMQTTTask();
void publishToMQTT(int relayIndex, bool state);

#endif // TASK_MQTT_H