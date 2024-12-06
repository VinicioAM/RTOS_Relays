#ifndef TASK_MQTT_H
#define TASK_MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void initializeMQTTTask();
void publishToMQTT(int relayIndex, bool state);

#endif // TASK_MQTT_H
