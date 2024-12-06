#include "TaskMQTT.h"
#include "ParametersConfig.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

static void mqttCallback(char *topic, byte *message, unsigned int length)
{
    // essa é a função de recebimento de msg no mqtt
    //  funciona OK

    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    //  exemplo log:
    //  Received message on topic: vinicio_tst
    // Message: oiii

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
    // essa função serve para eu ter a liberdade de publicar uma mensagem no mqtt

    Serial.println("antes do if");

    if (!mqttClient.connected())
    {
        Serial.println("dentro do if");

        Serial.println("MQTT not connected. Unable to publish message.");
        return;
    }
    Serial.println("apos do if");
    Serial.println("a");
    // aqui ta o vazamento
    mqttClient.beginPublish(MQTT_TOPIC, 50, false);
    // cabeçalho do method acima -> boolean PubSubClient::beginPublish(const char* topic, unsigned int plength, boolean retained) {
    mqttClient.print("Relay ");
    mqttClient.print(relayIndex);
    mqttClient.print(", Status = ");
    mqttClient.print(state);
    Serial.println("b");
    mqttClient.endPublish();
    // log quando tento chamar beginPublish
    //     Guru Meditation Error: Core  1 panic'ed (Unhandled debug exception).
    // Debug exception reason: Stack canary watchpoint triggered (RelayTask)
    // Core  1 register dump:
    // PC      : 0x4008ee23  PS      : 0x00060236  A0      : 0x8008e63f  A1      : 0x3ffb8a90
    // A2      : 0x3ffbf518  A3      : 0xb33fffff  A4      : 0x0000cdcd  A5      : 0x00060223
    // A6      : 0x00060223  A7      : 0x0000abab  A8      : 0xb33fffff  A9      : 0xffffffff
    // A10     : 0x3ffbd95c  A11     : 0x3ffb8c4c  A12     : 0x0000000c  A13     : 0x00000000
    // A14     : 0x007bf518  A15     : 0x003fffff  SAR     : 0x00000020  EXCCAUSE: 0x00000001
    // EXCVADDR: 0x00000000  LBEG    : 0x400898c0  LEND    : 0x400898cb  LCOUNT  : 0x00000000

    // Backtrace: 0x4008ee20:0x3ffb8a90 0x4008e63c:0x3ffb8ad0 0x4008cce8:0x3ffb8af0 0x400fb02e:0x3ffb8b30 0x400fb37d:0x3ffb8b50 0x400fc695:0x3ffb8b70 0x400fc721:0x3ffb8b90 0x400fc85c:0x3ffb8bc0 0x400fc8d2:0x3ffb8c10 0x400e9e7f:0x3ffb8c30 0x400eb5e1:0x3ffb8c70 0x400eb6b2:0x3ffb8cd0 0x400d3a34:0x3ffb8cf0 0x4014ba45:0x3ffb8d20 0x400d341f:0x3ffb8d40 0x400d34a1:0x3ffb8d60 0x400d2dba:0x3ffb8d80 0x400d29d5:0x3ffb8de0

    // ELF file SHA256: 7773e78eb3de8f96

    // Rebooting...
    // ets Jun  8 2016 00:22:57

    // rst:0xc (SW_CPU_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
    // configsip: 0, SPIWP:0xee
    // clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
    // mode:DIO, clock div:2
    // load:0x3fff0030,len:1184
    // load:0x40078000,len:13232
    // load:0x40080400,len:3028
    // entry 0x400805e4
    // MQTT client not connected. Cannot publish message.
}
