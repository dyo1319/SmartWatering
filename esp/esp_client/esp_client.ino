#include <DHT.h>
#include <ArduinoJson.h>

#define dhtPin 16
#define DHTTYPE DHT11

DHT dht(dhtPin, DHTTYPE);

#define lightSensor 34
#define MoistureSensore 36

unsigned long lastCheck = 0;
unsigned long checkInterval = 100000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi_SETUP();
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  updateStateFromServer();
  handleCurrentState();
  int light = analogRead(lightSensor);
  int moisture = analogRead(MoistureSensore);
  float t = dht.readTemperature();
  sendData(t, light, moisture);
  Serial.print("temp = ");
  Serial.print(t);
  Serial.print(", ligth = ");
  Serial.print(light);
  Serial.print(", Moisture = ");
  Serial.println(moisture);
  delay(500);
}
