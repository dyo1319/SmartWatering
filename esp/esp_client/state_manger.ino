float desiredTemp  = 25.0; 
int highTempDurationMinutes  = 10;
int lowTempDurationMinutes  = 5;

unsigned long lastWateringTime = 0;
unsigned long wateringInterval  = 12 * 60 * 60 * 1000; // watering every 12 hours miliseconds

// Define states
enum State {
  TEMPERATURE,
  HUMIDITY,
  SHABBAT,
  MANUAL
};

State currentState = TEMPERATURE;

void updateStateFromServer() {
    HTTPClient http;
    http.begin("http://172.20.10.2:4002/esp/state"); // כתובת ה-API
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
        String response = http.getString();
        Serial.println("Server Response: " + response);

        // ניתוח JSON כדי לעדכן את המצב
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);
        String state = doc["state"];

        if (state == "TEMPERATURE") {
            currentState = TEMPERATURE;
        } else if (state == "HUMIDITY") {
            currentState = HUMIDITY;
        } else if (state == "SHABBAT") {
            currentState = SHABBAT;
        } else if (state == "MANUAL") {
            currentState = MANUAL;
        }

        Serial.println("Updated state to: " + state);
    } else {
        Serial.println("Error in HTTP request: " + String(httpResponseCode));
    }

    http.end();
}

// Function to handle the current state
void handleCurrentState() {
  switch (currentState) {
    case TEMPERATURE:
      handleTemperatureState();
      break;
    case HUMIDITY:
      handleHumidityState();
      break;
    case SHABBAT:
      handleShabbatState();
      break;
    case MANUAL:
      handleManualState();
      break;
  }
}

void handleTemperatureState() {
    Serial.println("Handling Temperature State");

    float currentTemp = readTemperature();
    unsigned long currentTime = millis();

    if (currentTime - lastWateringTime >= wateringInterval) {
        if (currentTemp > desiredTemp) {
            Serial.println("🌡️ High temperature detected. Starting watering for high temp duration.");
            activatePump(highTempDurationMinutes);
        } else {
            Serial.println("❄️ Low temperature or equal. Starting watering for low temp duration.");
            activatePump(lowTempDurationMinutes);
        }
        lastWateringTime = currentTime;
    } else {
        Serial.println("⏳ Not time for watering yet.");
    }
}

float readTemperature() {
    return dht.readTemperature(); 
}

void activatePump(int durationInMinutes) {
    int durationInMillis = durationInMinutes * 60 * 1000; // המרת דקות למילישניות
    Serial.print("💧 Pump activated for ");
    Serial.print(durationInMinutes);
    Serial.println(" minutes.");

    // הפעלת המשאבה
    digitalWrite(PUMP_PIN, HIGH);
    delay(durationInMillis); // השהיה למשך הזמן המחושב
    digitalWrite(PUMP_PIN, LOW);

    Serial.println("💧 Pump deactivated.");
}

void handleHumidityState() {
  Serial.println("Handling Humidity State");
  // Add humidity-based logic here
}

void handleShabbatState() {
  Serial.println("Handling Shabbat State");
  // Add Shabbat-based logic here
}

void handleManualState() {
  Serial.println("Handling Manual State");
  // Add manual control logic here
}


