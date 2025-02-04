//--- libary ----
#include <DHT.h>
#include <ArduinoJson.h>

//----- pins ---------
#define lightSensor 34
#define MoistureSensore 36
#define dhtPin 16
#define pump 18


//---- General data -----
#define DHTTYPE DHT11
DHT dht(dhtPin,DHTTYPE);
StaticJsonDocument<200> doc;
float CurrentTemp;
int light;
int minutes = (1000 * 60);
float temp;
int minT,maxT;
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
  statusCheckTime = millis();
}

void loop() {
  // Update current state every 10 minutes
  if((millis() - statusCheckTime) > (1 * minutes)) {
    CurrentStatus = GetState();
    Serial.println(CurrentStatus);
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
        break;
    }
  delay(500);
}


// ---- HANDLE MANUAL MODE ----
void HandleManualMode() {
  static String lastCommand = "";

  String manualCommand = GetManualCommand();
  if (manualCommand == lastCommand) {
    return;
  }

  Serial.println("New manual command received: " + manualCommand);
  delay(3000); 

  static bool isPumpOn = false;
if (manualCommand == "on" && !isPumpOn) {
    Serial.println("Turning Pump ON...");
    digitalWrite(pump, HIGH);
    isPumpOn = true;
} else if (manualCommand == "off" && isPumpOn) {
    Serial.println("Turning Pump OFF...");
    digitalWrite(pump, LOW);
    isPumpOn = false;
}
  lastCommand = manualCommand;
}