#ifndef TASK_WIFI_H
#define TASK_WIFI_H

#include <WiFi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void initializeWiFiTask();
void reconnectWiFi();
void printWIFIStatus();

#endif // TASK_WIFI_H