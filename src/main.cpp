#include <Arduino.h>
#include "ParametersConfig.h"
#include "PinOut.h"
#include "TaskRelay.h"
#include "Switch.h"
#include "Relay.h"
#include "TaskWiFi.h"
#include "TaskMQTT.h"

Relay *relays[RELAYS_AND_SWITCHES_COUNT];
Switch *switches[RELAYS_AND_SWITCHES_COUNT];

void setup()
{
  Serial.begin(115200);

  relays[0] = new Relay(PIN_Output_RelayA);
  switches[0] = new Switch(PIN_Input_SwitchA);
  relays[1] = new Relay(PIN_Output_RelayB);
  switches[1] = new Switch(PIN_Input_SwitchB);

  pinMode(PIN_Input_SwitchA, INPUT);
  pinMode(PIN_Output_RelayA, OUTPUT);
  pinMode(PIN_Input_SwitchB, INPUT);
  pinMode(PIN_Output_RelayB, OUTPUT);
  pinMode(PIN_Output_WiFi_LED, OUTPUT);

  initializeRelayTask(relays, switches);
  initializeWiFiTask();
  initializeMQTTTask();
}

void loop()
{
  //
}
