#include <Arduino.h>
#include <unity.h>
#include "DimmableSwitch.h"

// Define o pino de teste
#define TEST_PIN 2

// Instância global da classe
DimmableSwitch dimmableSwitch(TEST_PIN);

// Funções de teste
void test_initial_state()
{
    TEST_ASSERT_EQUAL(LOW, dimmableSwitch.buttonState);
    TEST_ASSERT_EQUAL(255, dimmableSwitch.dutyCycle);
    TEST_ASSERT_FALSE(dimmableSwitch.ledState);
}

void test_button_press()
{
    // Simula leitura do botão pressionado
    pinMode(TEST_PIN, OUTPUT);
    digitalWrite(TEST_PIN, HIGH);
    delay(50); // Simula tempo de debounce
    TEST_ASSERT_TRUE(dimmableSwitch.detectarMudancaEstadoSwitch());
}

void test_brightness_adjustment()
{
    dimmableSwitch.holding();
    TEST_ASSERT_LESS_THAN(255, dimmableSwitch.dutyCycle); // Deve reduzir o brilho
}

// Setup do teste
void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_initial_state);
    RUN_TEST(test_button_press);
    RUN_TEST(test_brightness_adjustment);
    UNITY_END();
}

// Loop vazio, o Unity executa os testes no setup
void loop()
{
}
