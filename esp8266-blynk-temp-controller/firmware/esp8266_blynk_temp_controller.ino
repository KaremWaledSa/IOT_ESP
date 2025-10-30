/******************************************************
 * ESP8266 Blynk Temperature Controller
 * Board: NodeMCU 1.0 (ESP-12E)
 * Sensor: DHT11 or DHT22
 * Cloud: Blynk (New Platform)
 ******************************************************/
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include "secrets.h"
#define DHT_PIN D4
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);
#define PIN_HOT D6
#define PIN_COLD D5
#define VPIN_TEMP V0
#define VPIN_HUM V1
#define VPIN_THRESH V2
#define VPIN_HOT_LED V3
#define VPIN_COLD_LED V4
float thresholdC = 30.0;
const float HYSTERESIS_C = 1.0;
const unsigned long SENSOR_INTERVAL_MS = 2000;
const unsigned long CONTROL_INTERVAL_MS = 1000;
BlynkTimer timer;
float lastTemp = NAN;
float lastHum  = NAN;
bool hotOn = false;
bool coldOn = false;
BLYNK_WRITE(VPIN_THRESH) { thresholdC = param.asFloat(); }
void readAndPublishSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) return;
  lastTemp = t; lastHum  = h;
  Blynk.virtualWrite(VPIN_TEMP, t);
  Blynk.virtualWrite(VPIN_HUM,  h);
}
void controlLoop() {
  if (isnan(lastTemp)) return;
  if (lastTemp < (thresholdC - HYSTERESIS_C)) { hotOn = true; coldOn = false; }
  else if (lastTemp > (thresholdC + HYSTERESIS_C)) { hotOn = false; coldOn = true; }
  digitalWrite(PIN_HOT,  hotOn ? HIGH : LOW);
  digitalWrite(PIN_COLD, coldOn ? HIGH : LOW);
  Blynk.virtualWrite(VPIN_HOT_LED,  hotOn ? 255 : 0);
  Blynk.virtualWrite(VPIN_COLD_LED, coldOn ? 255 : 0);
}
void setup() {
  pinMode(PIN_HOT, OUTPUT); pinMode(PIN_COLD, OUTPUT);
  digitalWrite(PIN_HOT, LOW); digitalWrite(PIN_COLD, LOW);
  Serial.begin(115200); dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  timer.setInterval(SENSOR_INTERVAL_MS, readAndPublishSensor);
  timer.setInterval(CONTROL_INTERVAL_MS, controlLoop);
}
void loop() { Blynk.run(); timer.run(); }
