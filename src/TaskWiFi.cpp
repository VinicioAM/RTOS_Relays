#include "TaskWiFi.h"
#include "ParametersConfig.h"
#include "PinOut.h"

static String ipAddress;
static bool connected = false;

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
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay de 1 segundo
    }
}

void initializeWiFiTask()
{
    xTaskCreatePinnedToCore(
        wifiTask,   // Função da task
        "WiFiTask", // Nome da task
        4096,       // Tamanho da stack
        NULL,       // Parâmetros
        1,          // Prioridade
        NULL,       // Handle
        1           // Core para rodar a task
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
