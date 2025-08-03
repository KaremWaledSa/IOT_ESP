#include <ESP8266WiFi.h>
// #include <BlynkSimpleEsp8266.h>

// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL2cDdHCcaY"
#define BLYNK_TEMPLATE_NAME "Wemoss"
// #define BLYNK_AUTH_TOKEN "sbhCtgHmIg5Nn1oYKZ-LVq5EZb5eUx9y"

//#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#include "BlynkEdgent.h" 

#define APP_DEBUG
#define led D4

char ssid[] = "Id";
char pass[] = "Pswd";

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI


// BLYNK_WRITE(V2)
// {
//   analogWrite(led,param.asInt());
// }
BLYNK_WRITE(V0)
{
  digitalWrite(led,param.asInt());
  delay(100);
  digitalWrite(led,!(param.asInt()));
  delay(100);
}


BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V2);  
  Blynk.syncVirtual(V2);  

}

void setup()
{
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Wi-Fi connected, print IP address
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(led, OUTPUT); // Initialise digital pin 2 as an output pin
  Serial.begin(115200);
  delay(100);

  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
  // Blynk.run();
}
