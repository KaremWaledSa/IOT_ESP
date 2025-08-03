#include <ESP8266WiFi.h>


#define SOS D3
#define SLEEP_PIN D2

boolean stringComplete = false;
String inputString = "";
String fromGSM = "";
int c = 0;
String SOS_NUM = "+201067149792";

int SOS_Time = 5;

bool CALL_END = 1;
char* response = " ";
String res = "";

void setup() 
{
  // Making Radio OFF for power saving
  WiFi.mode(WIFI_OFF);
  pinMode(SOS, INPUT_PULLUP);
  pinMode(SLEEP_PIN, OUTPUT);

  Serial.begin(9600); // For Serial Monitor

  delay(20000);

  digitalWrite(SLEEP_PIN, LOW); // Sleep Mode OFF
  Serial.println("AT");               // Just Checking
  delay(1000);
  Serial.println("AT+GPS = 1");      // Turning ON GPS
  delay(1000);
  Serial.println("AT+GPSLP = 2");      // GPS low power
  delay(1000);
  Serial.println("AT+SLEEP = 1");    // Configuring Sleep Mode to 1
  delay(1000);
  digitalWrite(SLEEP_PIN, HIGH); // Sleep Mode ON
}

void loop() {
  if (Serial.available()) 
  {
    char inChar = Serial.read();
    if (inChar == '\n') 
    {
      // check the state
      if (fromGSM == "OK\r") 
      {
        Serial.println("---------IT WORKS-------");
      }
      else if (fromGSM == "RING\r")
      {
        digitalWrite(SLEEP_PIN, LOW); // Sleep Mode OFF
        Serial.println("---------ITS RINGING-------");
        Serial.println("ATA");
      }
      else if (fromGSM == "ERROR\r") {
        Serial.println("---------IT DOESN'T WORK-------");
      }
      else if (fromGSM == "NO CARRIER\r") {
        Serial.println("---------CALL ENDS-------");
        CALL_END = 1;
        digitalWrite(SLEEP_PIN, HIGH); // Sleep Mode ON
      }
      // write the actual response
      Serial.println(fromGSM);
      // clear the buffer
      fromGSM = "";
    } else {
      fromGSM += inChar;
    }
    delay(20);
  }

  // read from port 0, send to port 1:
  if (Serial1.available()) {
    char inChar = Serial1.read();
    Serial.write(inChar);
    if (inChar == '\n') {
      // check the state
      if (fromGSM == "OK\r") {
        Serial.println("---------IT WORKS-------");
      }
      else if (fromGSM == "RING\r") {
        digitalWrite(SLEEP_PIN, LOW); // Sleep Mode OFF
        Serial.println("---------ITS RINGING-------");
        Serial.println("ATA");
      }
      else if (fromGSM == "ERROR\r") {
        Serial.println("---------IT DOESN'T WORK-------");
      }
      else if (fromGSM == "NO CARRIER\r") {
        Serial.println("---------CALL ENDS-------");
        CALL_END = 1;
        digitalWrite(SLEEP_PIN, HIGH); // Sleep Mode ON
      }
      // write the actual response
      Serial.println(fromGSM);
      // clear the buffer
      fromGSM = "";
    }
    delay(20);
  }

  // When SOS button is pressed
  if (digitalRead(SOS) == LOW && CALL_END == 1) {
    Serial.print("Calling In.."); // Waiting for 5 sec
    for (c = 0; c < SOS_Time; c++) {
      Serial.println((SOS_Time - c));
      delay(1000);
      if (digitalRead(SOS) == HIGH)
        break;
    }
    if (c == 5) {
      // Getting Location and making Google Maps link
      digitalWrite(SLEEP_PIN, LOW);
      delay(1000);
      Serial.println("AT+LOCATION = 2");
      delay(2000);
      while (Serial1.available()) {
        char add = Serial1.read();
        res += add;
        delay(1);
      }
      res = res.substring(17, 38);
      response = &res[0];
      Serial.print("Received Data - ");
      Serial.println(response);
      Serial.println("\n");

      if (strstr(response, "GPS NOT")) {
        Serial.println("No Location data");
      }
      else {
        int i = 0;
        while (response[i] != ',')
          i++;

        String location = (String)response;
        String lat = location.substring(2, i);
        String longi = location.substring(i + 1);
        Serial.println(lat);
        Serial.println(longi);

        String Gmaps_link = ("http://maps.google.com/maps?q=" + lat + "+" + longi);

        // Sending SMS with Google Maps Link with our Location
        Serial1.println("AT+CMGF=1");
        delay(1000);
        Serial1.println("AT+CMGS=\"" + SOS_NUM + "\"\r");
        delay(1000);
        Serial1.print("I'm here " + Gmaps_link);
        delay(1000);
        Serial1.println((char)26);
        delay(1000);
      }
      response = "";
      res = "";

      // Calling on that same number after sending SMS
      Serial.println("Calling Now");
      Serial1.println("ATD" + SOS_NUM);
      CALL_END = 0;
    }
  }

  // only write a full message to the GSM module
  if (stringComplete) {
    Serial1.print(inputString);
    inputString = "";
    stringComplete = false;
  }
}
