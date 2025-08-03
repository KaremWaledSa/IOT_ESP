#define BoardSelect D13  //wemoss
// #define BoardSelect   6   //Node

#if BoardSelect == D13
#define TempPin D13
#elif BoardSelect == 6
#define TempPin 6
#endif


// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL2xmrJe1i_"
#define BLYNK_TEMPLATE_NAME "NodeBlinkLed"
#define BLYNK_FIRMWARE_VERSION "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

#include "DHT.h"
int DHTPIN = TempPin;
#define DHTTYPE DHT11  // DHT 11

float temperature;
float humidity;

DHT dht(DHTPIN, DHTTYPE);

BLYNK_WRITE(V0) {
  if (param.asInt() == 1) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);
  Blynk.virtualWrite(V2, temperature);
}

void setup() {

  pinMode(2, OUTPUT);  // Initialise digital pin 2 as an output pin
  Serial.begin(115200);
  delay(100);

  dht.begin();

  BlynkEdgent.begin();
}

void loop() {

  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temperature = dht.readTemperature();

  // // Check if any reads failed and exit early (to try again).
  // if (isnan(humidity) || isnan(temperature)) {
  //   Serial.println(F("Failed to read from DHT sensor!"));
  //   return;
  // }
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.println(F("°C "));
  delay(500);
  BlynkEdgent.run();
}
