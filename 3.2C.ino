#include "arduino_secrets.h"
/* 
  NAME: Pratham Garg
  ID: 2110994808

  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/820024b0-7eae-468f-9c3b-9f94803d61ae 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float light_intensity;
  bool sunlight;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/
#include <BH1750FVI.h>
#include <Wire.h>

// Create the Lightsensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

#include "thingProperties.h"

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH1   = "/trigger/light_present/with/key/mhCw1V_BRz9AbuAdJzRh3PGTl7GjY1bjj0yUXK2tfiT"; // change your EVENT-NAME and YOUR-KEY
String PATH2   = "/trigger/light_absent/with/key/mhCw1V_BRz9AbuAdJzRh3PGTl7GjY1bjj0yUXK2tfiT"; // change your EVENT-NAME and YOUR-KEY
String queryString = "?value1=57&value2=25";

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  LightSensor.begin();
  Wire.begin();
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  light_intensity=LightSensor.GetLightIntensity();
  Serial.print("Light: ");
  Serial.println(light_intensity);
  delay(250);
  if (light_intensity < 500 && sunlight == 0)
  {
    sunlight = 1;
    light_absent();
  }
  else if (light_intensity > 500 && sunlight == 1)
  {
    sunlight = 0;
    light_present();
  }
}

void light_present()
{
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to server");
  }
  else {
    Serial.println("Connection Failed");
  }

  client.println("GET " + PATH1 + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println();

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }

  client.stop();
  Serial.println();
  Serial.println("disconnected");
}

void light_absent()
{
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to server");
  }
  else {
    Serial.println("Connection Failed");
  }

  client.println("GET " + PATH2 + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println();

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }

  client.stop();
  Serial.println();
  Serial.println("disconnected");
}
/*
  Since Sunlight is READ_WRITE variable, onSunlightChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onSunlightChange()  {
  // Add your code here to act upon Sunlight change
}


