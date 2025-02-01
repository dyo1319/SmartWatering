//--- libary ----
#include <DHT.h>
#include <ArduinoJson.h>

//----- pins ---------
#define lightSensor 34
#define MoistureSensore 36
#define dhtPin 127366
#define pump 19


//---- General data -----
#define DHTTYPE DHT11
DHT dht(dhtPin,DHTTYPE);
StaticJsonDocument<200> doc;
float CurrentTemp;
int light;
int minutes = (1000 * 60);
float temp;
int minT,maxT;
bool isOnPump;
int countOn = 0;



//--------- State machine ----------
#define TEMP_MODE 61
#define SOIL_MOSTURE_MODE 62
#define SHABBAT_MODE 63
#define MANUAL_MODE 64
int CurrentStatus;
unsigned long statusCheckTime;
unsigned long DataPullTime;
unsigned long activationTime;


unsigned long lastCheck = 0;
unsigned long checkInterval = 100000;

void setup() {
  Serial.begin(115200);
  pinMode(pump,OUTPUT);
  WiFi_SETUP();
  dht.begin();
  isOnPump = true;
  statusCheckTime = millis();
}

void loop() {
  // Update current state every 10 minutes
  if((millis() - statusCheckTime) > (10 * minutes)) {
    CurrentStatus = GetState();
    statusCheckTime = millis();
  }
    switch (CurrentStatus) {
        case TEMP_MODE: 
            HandleTemperatureMode();
            break;
        case SOIL_MOSTURE_MODE: 
            HandleMoistureMode();
            break;
        case SHABBAT_MODE: 
            HandleShabbatMode();
            break;
        case MANUAL_MODE: 
            HandleManualMode();
            break;
        default:
            Serial.println("unknown state");
            break;
    }
  delay(500);
}

void HandleManualMode() { }
void HandleShabbatMode() {}
void HandleMoistureMode() {}
void HandleTemperatureMode() {}