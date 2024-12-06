#include <Arduino.h>
#include "ParametersConfig.h"
#include "PinOut.h"
#include "TaskRelay.h"
#include "Switch.h"
#include "Relay.h"
#include "TaskWiFi.h"
#include "TaskMQTT.h"

// Vetores para armazenar as instâncias de relés e switches
Relay *relays[RELAYS_AND_SWITCHES_COUNT];
Switch *switches[RELAYS_AND_SWITCHES_COUNT];

void setup()
{
  Serial.begin(115200);

  // Configura as instâncias dos relés e switches
  relays[0] = new Relay(PIN_Output_RelayA);    // GPIO 2 para o primeiro relé
  switches[0] = new Switch(PIN_Input_SwitchA); // GPIO 12 para o primeiro switch
  relays[1] = new Relay(PIN_Output_RelayB);    // GPIO 4 para o segundo relé
  switches[1] = new Switch(PIN_Input_SwitchB); // GPIO 14 para o segundo switch

  // Configura o pino do LED como saída
  pinMode(PIN_Input_SwitchA, INPUT);
  pinMode(PIN_Output_RelayA, OUTPUT);
  pinMode(PIN_Input_SwitchB, INPUT);
  pinMode(PIN_Output_RelayB, OUTPUT);
  pinMode(PIN_Output_WiFi_LED, OUTPUT);

  initializeRelayTask(relays, switches);
  initializeWiFiTask();
  // initializeMQTTTask();
}

void loop()
{
  //
}
