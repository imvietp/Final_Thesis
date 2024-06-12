// BLYNK
#define BLYNK_TEMPLATE_ID "TMPLI6ED0BTb" //mã định danh cho mẫu (template) dự án trong Blynk
#define BLYNK_DEVICE_NAME "Test Blynk"   //tên của thiết bị được hiển thị trong ứng dụng Blynk khi thiết bị kết nối thành công.
#define BLYNK_AUTH_TOKEN "rDDQy9mFrmA0uuvOB_EZdHG2NqzP-G-B" //mã thông báo (token) xác thực của thiết bị khi kết nối với Blynk. Mã thông báo này được sử dụng để xác thực và cho phép truy cập vào các tài nguyên và chức năng của dự án Blynk.



// Add library
#include "DHT.h"
#include <WiFi.h>                  //thư viện wifi
#include <BlynkSimpleEsp32.h>      //thư viện blynk



// Define pin
#define RXp2 16
#define TXp2 17
#define DHTPIN 4    // what digital pin the DHT sensor is connected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);



// define variable
int pump, val; 



// Wifi username and password
char auth[] = BLYNK_AUTH_TOKEN;                       // token (chứng thực) của blynk 
char ssid[] = "vietpham";                         // Wi-Fi network SSID
char pass[] = "11111111";                       // Wi-Fi network password



// Set button in blynk
BLYNK_WRITE(V3) // Button Widget writes to Virtual Pin V5 (nút trong blynk)
{
      pump = param.asInt();  
}
BLYNK_WRITE(V4) // Button Widget writes to Virtual Pin V5 (nút trong blynk)
{
      val = param.asInt();  
}

// Setup function
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); // Hardware Serial of ESP32
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
  manualmode();
  //float t = dht.readTemperature(); // Read temperature value
  //float h = dht.readHumidity();
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
  Blynk.virtualWrite(V0, humidity);             // Send temperature value to virtual pin V2 in Blynk app
  Blynk.virtualWrite(V1, celciusTemp);             // Send temperature value to virtual pin V2 in Blynk app

}

// manual mode
void manualmode() {
  if(pump == 1)                                    // nếu pump(switch) mức cao
  {
    //digitalWrite(BOM, HIGH);                      // kích relay máy bơm
    Serial2.write('A'); // Send 'a' character
  }
  else                                             // nếu pump mức thấp
  {
    //digitalWrite(BOM, LOW);                        // tắt máy bơm     
    Serial2.write('a'); // Send 'a' character
  }
   if (val == 1)                                   // nếu xả (switch) múc cao
  {  
    //digitalWrite(XA, HIGH);                       // Bật xả nước
    Serial2.write('B'); // Send 'a' character
  }
  else                                             // nếu xả mức thấp
  {
    // digitalWrite(XA, LOW);                        // tắt xả
    Serial2.write('b'); // Send 'a' character
  }
}