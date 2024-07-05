#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

    // Replace with your network credentials
const char* ssid = "Airtel_BhandGogi";
const char* password = "admin@123";

// ThingSpeak settings
unsigned long myChannelNumber = 2588969;
const char * myWriteAPIKey = "CAQC5K0XYETOK4IL";
// DHT11 sensor settings
#define DHTPIN D1  // Pin where the DHT11 is connected
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Read temperature and humidity values
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again)
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  // Write data to ThingSpeak
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  // Write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // Shorter delay (be cautious with ThingSpeak rate limits)
  delay(5000);
}
