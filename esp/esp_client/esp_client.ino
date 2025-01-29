#include <DHT.h>
#include <ArduinoJson.h>

#define lightSensor 34
#define MoistureSensore 36

#define dhtPin 16
#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

#define PUMP_PIN 12

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
  delay(500);
}
