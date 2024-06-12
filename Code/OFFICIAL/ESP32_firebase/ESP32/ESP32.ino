// ................... Add library .........................
#include <FirebaseESP32.h>         //thư viện firebase
#include "DHT.h"
#include <WiFi.h>                  //thư viện wifi

// ........................ Firebase .........................
#define FIREBASE_HOST "pigmanager-b0507-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "QRuGipNsMYyZUX5X1EoQN5Fd656UkkEmseZXPZW0"

//Define FirebaseESP32 data object
FirebaseData fbdo;


// .................. Define pin .........................
#define RXp2 16
#define TXp2 17
#define DHTPIN 4    // what digital pin the DHT sensor is connected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);
#define sensorPin 35


// define variable
int pump, val; 
float underThreshold = 25.0; // Initial temperature threshold
float upperThreshold = 35.0; // Initial temperature threshold
int humidityUnderThreshold = 50;
int smokeThreshold = 0; // Initialize smoke threshold variable
unsigned long t1 = 0;

// Wifi username and password
char ssid[] = "Minh Hoang";                         // Wi-Fi network SSID
char pass[] = "minhhoang1997";                       // Wi-Fi network password



// Setup function
void setup() {
  Serial.begin(250000);
  Serial2.begin(250000, SERIAL_8N1, RXp2, TXp2); // Hardware Serial of ESP32
  Serial.println("DHTxx test!");
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // ............. Kết nối Wifi .............................
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // .................... kết nối firebase .............................
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(fbdo, 1000 * 60);   // Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setwriteSizeLimit(fbdo, "tiny");   // Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  
  dht.begin();
}



// Main function
void loop() {
  delay(200);
  dht11_function(); // Read DHT11 sensor data
  readSensorMQ2() ;
  // manualmode();
  //float t = dht.readTemperature(); // Read temperature value
  //float h = dht.readHumidity();
}



// xxxxxxxxx dht11 function xxxxxxxxxxxxxxx
void dht11_function() {
  // Wait a few seconds between measurements.
  // Read sensor data
  float humidity = dht.readHumidity();
  float celciusTemp = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(celciusTemp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print sensor data
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" % ");
  Serial.print("Temperature: ");
  Serial.print(celciusTemp);
  Serial.println(" *C");

  // Update Firebase data
  Firebase.setInt(fbdo, "/Finalproject/temperature/temp", celciusTemp);
  Firebase.setInt(fbdo, "/Finalproject/humidity/humid", humidity);

  // Read temperature and humidity thresholds from Firebase
  if (Firebase.getInt(fbdo, "/Finalproject/temperature/thresholds/underThreshold")) {
    underThreshold = fbdo.intData();
    Serial.print("Temperature threshold updated to: ");
    Serial.println(underThreshold);
  } else {
    Serial.println("Failed to get lower temperature threshold from Firebase!");
  }

  if (Firebase.getInt(fbdo, "/Finalproject/temperature/thresholds/upperThreshold")) {
    upperThreshold = fbdo.intData();
    Serial.print("Temperature threshold updated to: ");
    Serial.println(upperThreshold);
  } else {
    Serial.println("Failed to get upper temperature threshold from Firebase!");
  }

  if (Firebase.getInt(fbdo, "/Finalproject/humidity/thresholds/underThreshold")) {
    humidityUnderThreshold = fbdo.intData();
    Serial.print("Humidity threshold updated to: ");
    Serial.println(humidityUnderThreshold);
  } else {
    Serial.println("Failed to get lower humidity threshold from Firebase!");
  }

  // Send appropriate characters over Serial2 based on temperature and humidity thresholds
  if (celciusTemp < underThreshold) {
    Serial.println("Temperature below lower threshold!");
    Serial2.write('A'); // Send 'A' character
  } else if (celciusTemp > upperThreshold) {
    Serial.println("Temperature exceeds upper threshold!");
    Serial2.write('B'); // Send 'B' character
  } else {
    Serial2.write('a'); // Send 'a' character for normal temperature
  }

  if (humidity < humidityUnderThreshold) {
    Serial.println("Humidity below threshold!");
    Serial2.write('C'); // Send 'C' character
  } else {
    Serial2.write('c'); // Send 'c' character for normal humidity
  }
}

int readSensorMQ2() {

  unsigned int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor

  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255); // map the 10-bit data to 8-bit data

    // Update Firebase data
  Firebase.setInt(fbdo, "/Finalproject/MQ2/data", outputValue);

  if (Firebase.getInt(fbdo, "/Finalproject/MQ2/threshold")) {
    smokeThreshold = fbdo.intData();
  }

  if (outputValue > smokeThreshold) {
    Serial.println("Smoke detected!"); // Print debug message
    Serial2.write('D'); // Send 'D' character for smoke detection
  }
  else 
  {
    Serial2.write('d'); // Send 'D' character for smoke detection
  }

}







