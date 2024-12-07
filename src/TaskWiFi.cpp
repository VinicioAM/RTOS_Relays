#include "TaskWiFi.h"
#include "ParametersConfig.h"
#include "PinOut.h"

static String ipAddress;
static bool connected = false;

void reconnectWiFi()
{
    WiFi.reconnect();
}

void wifiTask(void *param)
{

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connecting to Wi-Fi...");

    while (true)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            if (!connected)
            {
                connected = true;
                ipAddress = WiFi.localIP().toString();
                Serial.println("Connected to Wi-Fi.");
                Serial.println("IP Address: " + ipAddress);
            }
        }
        else
        {
            if (connected)
            {
                connected = false;
                Serial.println("Disconnected from Wi-Fi. Reconnecting...");
                WiFi.reconnect();
            }
        }
        digitalWrite(PIN_Output_WiFi_LED, connected);
        vTaskDelay(TIMESTEP_WIFI / portTICK_PERIOD_MS); // Delay de 1 segundo
    }
}

void initializeWiFiTask()
{
    xTaskCreatePinnedToCore(
        wifiTask,   // Task function
        "WiFiTask", // Task name
        4096,       // Stack size
        NULL,       // Parameters
        1,          // Priority
        NULL,       // Handle
        1           // Core to run the task
    );
}

bool isWiFiConnected()
{
    return connected;
}

String getWiFiIPAddress()
{
    return ipAddress;
}
