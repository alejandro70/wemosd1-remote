#include <Arduino.h>

/*
* SuperRemote V1.0
* Generador de secuencias de control remoto para control de dsispositivos
* https://www.instructables.com/id/Universal-Remote-Using-ESP8266Wifi-Controlled/
*/

// IR library
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define PIN_TOUCH 2    // touch sensor
#define PIN_BTN1 15    // the pin for button 1
#define PIN_BTN2 16    // the pin for button 2
#define LED_BUILTIN 13 // LED Indicador
#define PIN_IR_LED D2  // Infra-Red LED

#define RM_AMU178_POWER 0xA81
#define RM_AMU178_SLEEP 0x61
#define RM_AMU178_VOLUME_UP 0x481
#define RM_AMU178_VOLUME_DOWN 0xC81
#define RM_AMU178_MEMORY_1 0x001
#define RM_AMU178_MEMORY_2 0x801
#define RM_AMU178_MEMORY_3 0x401
#define RM_AMU178_MEMORY_4 0xC01
#define RM_AMU178_MEMORY_5 0x201
#define RM_AMU178_MEMORY_6 0xA01

/* ---------- DECLARACION DE VARIABLES ---------- */
IRsend irsend(PIN_IR_LED);           // An IR LED is controlled by GPIO pin 4 (D2)
volatile bool touch_state = false;   // ID of the pin has changed
void Send_AMU178(unsigned long key); // envío IR code
void TurnOn_AMU178(void);

void setup()
{
  // configure pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_IR_LED, OUTPUT);
  digitalWrite(PIN_IR_LED, LOW);

  irsend.begin();

#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif // ESP8266

  TurnOn_AMU178();

  // Deep sleep mode until RESET pin is connected to a LOW signal
  ESP.deepSleep(0);
}

void loop()
{
}

void TurnOn_AMU178(void)
{
  int i;
  Send_AMU178(RM_AMU178_POWER);
  delay(7000);
  for (i = 0; i < 9; i++)
  {
    Send_AMU178(RM_AMU178_SLEEP);
  }
}

void Send_AMU178(unsigned long key)
{
  for (int i = 0; i < 3; i++)
  {
    irsend.sendSony(key, 12);
    delay(40);
  }
  delay(100);
}