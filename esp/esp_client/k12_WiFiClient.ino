#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

// WiFi credentials
//const char* ssid = "Kinneret College";

const char* ssid = "iPhone (6)";
const char* password = "95175386240";

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


