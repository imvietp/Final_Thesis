#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FirebaseESP32.h>
#include "DHT.h"


// ........................ Firebase .........................
#define FIREBASE_HOST "pigmanager-b0507-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "QRuGipNsMYyZUX5X1EoQN5Fd656UkkEmseZXPZW0"


// .............................................................
const char* ssid = "vietpham";
const char* password = "11111111";
const long utcOffsetInSeconds = 7 * 3600;  // Adjust this based on your timezone
FirebaseData fbdo;


// ......................... Define pins ...........................
#define RXp2 16
#define TXp2 17
#define DHTPIN 4
#define led 18
#define led_an 19
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// ................... Variables .......................................
int tempUnderThreshold = 25; 
int tempUpperThreshold = 35; 
int humidityUnderThreshold = 20;
int valueTemp;
int valueHumidity;

unsigned long lastThresholdCheckTime = 0;
unsigned long lastLightCheckTime = 0;
unsigned long lastFeedingCheckTime = 0;
const unsigned long thresholdCheckInterval = 1000; // Interval for threshold check (in milliseconds)
const unsigned long lightCheckInterval = 1000; // Interval for light check (in milliseconds)
const unsigned long feedingCheckInterval = 1000;  // Interval for feeding check (in milliseconds)
unsigned long feedingStartTime = 0; // Variable to store the start time of feeding


// ........................ Setup ........................................
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); // Hardware Serial of ESP32
  WiFi.begin(ssid, password);
  pinMode(led, OUTPUT);
  pinMode(led_an, OUTPUT);
  digitalWrite(led, LOW);
  digitalWrite(led_an, LOW);
  delay(500);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  Firebase.setwriteSizeLimit(fbdo, "tiny");

  dht.begin();

}


void loop() {
  unsigned long currentTime = millis();

  // Check if it's time to perform threshold check
  if (currentTime - lastThresholdCheckTime >= 20000) {
    //Serial.println("Performing threshold check...");
    updateFirebase();
    lastThresholdCheckTime = currentTime;
  }
  checkThresholdsAndControlDevices();


  // Check if it's time to perform light check
  if (currentTime - lastLightCheckTime >= lightCheckInterval) {
    turnonoffLight();
    lastLightCheckTime = currentTime;
  }

  // Debug thông tin về thời gian kiểm tra việc cho ăn

  if (currentTime - lastFeedingCheckTime >= feedingCheckInterval) {
    turnonoffFeeding();
    lastFeedingCheckTime = currentTime;
  }
}


void updateFirebase()
{
  valueHumidity = dht.readHumidity();
  valueTemp = dht.readTemperature();

  Firebase.setInt(fbdo, "/CAGE1/temperature/temp", valueTemp);
  Firebase.setInt(fbdo, "/CAGE1/humidity/humid", valueHumidity);
}


void checkThresholdsAndControlDevices() {
  bool tempFlag1 = false; 
  bool tempFlag2 = false; 
  bool humidityFlag = false; 
  updateFirebase();

  if (Firebase.getInt(fbdo, "/CAGE1/temperature/thresholds/tempUnderThreshold"))
  {
    tempUnderThreshold = fbdo.intData();
  }

  if (Firebase.getInt(fbdo, "/CAGE1/temperature/thresholds/tempUpperThreshold")) 
  {
    tempUpperThreshold = fbdo.intData();
  }

  if (Firebase.getInt(fbdo, "/CAGE1/humidity/thresholds/humidUnderThreshold")) 
  {
    humidityUnderThreshold = fbdo.intData();
  }

  // Read current temperature and humidity from the sensor
  valueTemp = dht.readTemperature();
  valueHumidity = dht.readHumidity();

  
  // Check temperature conditions
  if (valueTemp < tempUnderThreshold) 
  {
    tempFlag1 = true; // Temperature is below lower threshold
    tempFlag2 = false;
  } 
  else if (valueTemp > tempUpperThreshold) 
  {
    tempFlag2 = true;
    tempFlag1 = false; // Temperature is below lower threshold
  } 
  else 
  {
    tempFlag1 = false; // Temperature is below lower threshold
    tempFlag2 = false;
  }


  if (valueHumidity < humidityUnderThreshold) 
  {
    humidityFlag = true; 
  } 
  else
  {
    humidityFlag = false; 
  }


  if (tempFlag1) 
  {
    Serial2.write('A'); // Send 'A' character for low temperature
  } 
  else if (tempFlag2)
  {
    Serial2.write('B'); // Send 'B' character for high temperature
  } 
  else 
  {
    Serial2.write('a'); // Send 'a' character for normal temperature
  }


  if (humidityFlag) 
  {
    Serial2.write('C'); 
  } 
  else 
  {
    Serial2.write('c'); 
  }
}



void turnonoffLight() {
  static bool ledInProgress = false;
  if (Firebase.getString(fbdo, "/CAGE1/Light/Status")) {
    String currentStatusLed = fbdo.stringData();
    if (currentStatusLed == "ON") {
      if (!ledInProgress) {
        ledInProgress = true;
        Serial2.write('D'); // Turn on the light
      } 
    }
    else {
      if (ledInProgress) {
        ledInProgress = false;
        Serial2.write('d'); // Turn off the light
      }    
    }
  }  
}



void turnonoffFeeding() {
  static bool feedingInProgress = false;
  static unsigned long feedingStartTime = 0;

  if (Firebase.getString(fbdo, "/CAGE1/Feeding/Status")) {
    String feedingStatus = fbdo.stringData();
    if (feedingStatus == "ON") {
      if (!feedingInProgress) {
        feedingInProgress = true;
        feedingStartTime = millis(); // Ghi lại thời gian bắt đầu cho việc cho ăn
        Serial2.write('E'); // Bật cho ăn
      } else if (millis() - feedingStartTime >= 5000) {
        feedingInProgress = false;
        Serial2.write('e'); // Tắt cho ăn sau 5 giây
        Firebase.setString(fbdo, "/CAGE1/Feeding/Status", "OFF"); // Gửi trạng thái OFF về Firebase
      }
    } else {
      if (feedingInProgress) {
        feedingInProgress = false;
        Serial2.write('e'); // Tắt cho ăn nếu đang trong quá trình cho ăn
      }
    }
  }
}

