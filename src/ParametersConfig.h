#ifndef PARAMETERS_CONFIG
#define PARAMETERS_CONFIG

#define TIMESTEP_MQTT 60
#define TIMESTEP_RELAY 40
#define TIMESTEP_WIFI 1000
#define RELAYS_AND_SWITCHES_COUNT 2
#define DIMMERS_AND_DIMMABLESWITCHES_COUNT 1
#define DEBOUNCE_TIME 60
#define DEBOUNCE_TIME_ALEXA_SHORT 400
#define DEBOUNCE_TIME_ALEXA_MEDIUM 1500
#define DEBOUNCE_TIME_ALEXA_LONG 35000
#define PRINT_IP_DELAY 20000
#define BLINK_DELAY 350
#define PWM_CHANNEL 0
#define PWM_FREQ 1000
#define PWM_RESOL 8
#define BRIGHTNESS_ADJUST_STEP 5      // Step for decreasing brightness
#define BRIGHTNESS_ADJUST_INTERVAL 50 // Time between each brightness decrease
#define DOUBLE_CLICK_DELAY 200        // Max time between clicks
#define HOLD_DELAY 300                // Time to detect a hold
#define WIFI_SSID "brisa-2270812"
#define WIFI_PASSWORD "onx8rrcr"
#define MQTT_BROKER_IP "192.168.0.13"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC_IN "from_MQTT_to_device"
#define MQTT_TOPIC_OUT "from_device_to_relay"
#define MQTT_QOS 2

#endif // PARAMETERS_CONFIG