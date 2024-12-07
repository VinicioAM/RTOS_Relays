#include <Arduino.h>
#include "ParametersConfig.h"
#include "PinOut.h"
#include "TaskRelay.h"
#include "Switch.h"
#include "Relay.h"
#include "TaskWiFi.h"
#include "TaskMQTT.h"
#include "TaskSerial.h"

Relay *relays[RELAYS_AND_SWITCHES_COUNT];
Switch *switches[RELAYS_AND_SWITCHES_COUNT];

void setup()
{
  initializeSerialMonitorTask();
  // RelayA
  pinMode(PIN_Input_SwitchA, INPUT);
  pinMode(PIN_Output_RelayA, OUTPUT);
  relays[0] = new Relay(PIN_Output_RelayA);
  switches[0] = new Switch(PIN_Input_SwitchA);
  // RelayB
  pinMode(PIN_Input_SwitchB, INPUT);
  pinMode(PIN_Output_RelayB, OUTPUT);
  relays[1] = new Relay(PIN_Output_RelayB);
  switches[1] = new Switch(PIN_Input_SwitchB);

  pinMode(PIN_Output_WiFi_LED, OUTPUT);

  initializeRelayTask(relays, switches);
  initializeWiFiTask();
  initializeMQTTTask();
}

void loop()
{
  //
}
