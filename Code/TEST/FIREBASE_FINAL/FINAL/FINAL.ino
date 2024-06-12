// Add library
#include <FirebaseESP32.h>         //thư viện firebase
#include <WiFi.h>                  //thư viện wifi
#include "DHT.h"

// define pin
#define DHTPIN 4     // what digital pin the DHT sensor is connected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
#define FIREBASE_HOST "finalproject2-1ad29-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "fr79KwRhPTzrXpDOKuLkwvncIuWbYCnzhAWNFnRU"

//Define FirebaseESP32 data object
FirebaseData fbdo;

char ssid[] = "Quoc Viet";                         // Wi-Fi network SSID
char pass[] = "quocviet2002";                       // Wi-Fi network password            // Wi-Fi network password


// SETUP FUNCTION
void setup() {
  // initial serial monitor
  Serial.begin(115200); 

  // Kết nối Wifi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

    //kết nối firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);


  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(fbdo, "tiny");

  dht.begin();
}



void loop() {
  // put your main code here, to run repeatedly:

}



void dht11_function(){
  // Wait a few seconds between measurements.
  delay(2000);
  float humid = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float celciusTemp = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float fahrenheitTemp = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(humid) || isnan(celciusTemp) || isnan(fahrenheitTemp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  
  // Compute heat index in Celsius (Fahrenheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("  Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}
