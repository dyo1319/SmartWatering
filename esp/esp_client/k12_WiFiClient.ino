#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
//const char* ssid = "Kinneret College";

const char* ssid = "iPhone (6)";
const char* password = "95175386240";

//const char* ssid = "RaadHeno";
//const char* password = "Raad12340";

WiFiClient client;

void WiFi_SETUP(){
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void sendData(float temp, int linght, int moisture){
  HTTPClient http;
  String dataUrl = "temp=" + String(temp);
  dataUrl+= "&linght="+ String(linght);
  dataUrl+= "&moisture="+ String(moisture);
  http.begin(client, "http://172.20.10.2:4002/esp?" + dataUrl );

   int httpCode = http.GET();
   if(httpCode == HTTP_CODE_OK) {
     Serial.print("HTTP response code ");
     Serial.println(httpCode);
     //String Res = http.getString();
     //Serial.println(Res);
     //ret = Res.toInt();
    }
    http.end();
}

int GetState() {
    HTTPClient http;

    http.begin("http://172.20.10.2:4002/esp/state"); 
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Raw Response: " + response);  // הדפסת תגובה גולמית

        // בדוק אם התגובה אינה ריקה
        if (response.length() == 0) {
            Serial.println("Empty response received!");
            return -1;
        }

        // פירוש JSON
        DynamicJsonDocument doc(256);
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            Serial.print("JSON Parse Error: ");
            Serial.println(error.f_str());
            return -1;
        }

        // שליפת ה-state
        int state = doc["state"];
        Serial.print("Parsed State: ");
        Serial.println(state);

        return state;
    } else {
        Serial.print("HTTP Error: ");
        Serial.println(httpResponseCode);
        return -1; 
    }

    http.end(); 
}


// ---- GET MANUAL COMMAND ----
String GetManualCommand() {
  HTTPClient http;
  http.begin("http://172.20.10.2:4002/esp/manual");
  int httpCode = http.GET();
  String command = "";

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    if (response.indexOf("on") != -1) {
      command = "on";
    } else if (response.indexOf("off") != -1) {
      command = "off";
    }
  } else {
    Serial.println("Failed to get manual command. HTTP Code: " + String(httpCode));
  }

  http.end();
  return command;
}



void HandleShabbatMode() {}
void HandleMoistureMode() {}
void HandleTemperatureMode() {}