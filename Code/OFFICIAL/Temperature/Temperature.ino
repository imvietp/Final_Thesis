// BLYNK
#define BLYNK_TEMPLATE_ID "TMPLI6ED0BTb" //mã định danh cho mẫu (template) dự án trong Blynk
#define BLYNK_DEVICE_NAME "Test Blynk"   //tên của thiết bị được hiển thị trong ứng dụng Blynk khi thiết bị kết nối thành công.
#define BLYNK_AUTH_TOKEN "rDDQy9mFrmA0uuvOB_EZdHG2NqzP-G-B" //mã thông báo (token) xác thực của thiết bị khi kết nối với Blynk. Mã thông báo này được sử dụng để xác thực và cho phép truy cập vào các tài nguyên và chức năng của dự án Blynk.



// Add library
#include "DHT.h"
#include <WiFi.h>                  //thư viện wifi
#include <BlynkSimpleEsp32.h>      //thư viện blynk



// Define pin
#define DHTPIN 3    // what digital pin the DHT sensor is connected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);



// Wifi username and password
char auth[] = BLYNK_AUTH_TOKEN;                       // token (chứng thực) của blynk 
char ssid[] = "mitom2trung";                         // Wi-Fi network SSID
char pass[] = "66666666";                       // Wi-Fi network password



// Setup function
void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Kết nối Wifi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Blynk.begin(auth, ssid, pass);
  dht.begin();
}



// Main function
void loop() {
  dht11_function(); // Read DHT11 sensor data
  float t = dht.readTemperature(); // Read temperature value
  float h = dht.readHumidity();
}



// dht11 function
void dht11_function(){
  // Wait a few seconds between measurements.
  delay(2000);
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float celciusTemp = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float fahrenheitTemp = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(celciusTemp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (Fahrenheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" % ");
  Serial.print("Temperature: ");
  Serial.print(celciusTemp);
  Serial.print(" *C\n");

}