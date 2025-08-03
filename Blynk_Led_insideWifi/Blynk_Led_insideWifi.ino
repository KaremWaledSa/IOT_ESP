
#define BLYNK_TEMPLATE_ID           "TMPL2cDdHCcaY"
#define BLYNK_TEMPLATE_NAME         "Wemoss"
#define BLYNK_AUTH_TOKEN            "sbhCtgHmIg5Nn1oYKZ-LVq5EZb5eUx9y"

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define led D4

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ID";
char pass[] = "PW";

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
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(led, OUTPUT); 
}

void loop()
{
  Blynk.run();
}
