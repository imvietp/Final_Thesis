#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FirebaseESP32.h>
#include "DHT.h"


// ........................ Firebase .........................
#define FIREBASE_HOST "pigmanager-b0507-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "QRuGipNsMYyZUX5X1EoQN5Fd656UkkEmseZXPZW0"



const char* ssid = "vietpham";
const char* password = "11111111";
const long utcOffsetInSeconds = 7 * 3600;  // Adjust this based on your timezone
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
FirebaseData fbdo;


#define RXp2 16
#define TXp2 17
#define DHTPIN 4
#define DHTTYPE DHT11
#define sensorPin 35
#define maysuoi 15
#define quattannhiet 5
#define phunsuong 13
#define led 18
#define led_an 19

DHT dht(DHTPIN, DHTTYPE);
unsigned long previousMillisNTP = 0;
unsigned long currentMillis = 0;
unsigned long currentMillisDHT11 = 0;
unsigned long previousMillisDHT11 = 0;
unsigned long previousMillis = 0;


float prevHumidity = 20;
float prevCelciustemp = 20;

// Khai báo biến để lưu trữ các ngưỡng cục bộ


//int underThreshold = 25; // Initial temperature threshold
//int upperThreshold = 35; // Initial temperature threshold
//int humidityUnderThreshold = 50;



int smokeThreshold = 0;
unsigned long ts = 0, ts1 = 0, ts2 = 0;
unsigned long t1 = 0, t2 = 0;
int GTS = 0, GTS2 = 0;
int GT = 0, GT2 = 0;
int k = 0, k2 = 0;
byte giocd, phutcd, giaycd;
byte giocd2, phutcd2, giaycd2;
byte gio, phut, giay;
byte gio2, phut2, giay2;
bool enableAn = false;
bool enableLed = false;
bool tatdenValue = false, tatanValue = false;



void setup() {
  Serial.begin(250000);
  Serial2.begin(250000, SERIAL_8N1, RXp2, TXp2); // Hardware Serial of ESP32
  WiFi.begin(ssid, password);
  pinMode(maysuoi, OUTPUT);
  pinMode(quattannhiet, OUTPUT);
  pinMode(phunsuong, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(led_an, OUTPUT);
  delay(5000);


  if (WiFi.status() == WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }


  Firebase.begin("pigmanager-b0507-default-rtdb.asia-southeast1.firebasedatabase.app", "QRuGipNsMYyZUX5X1EoQN5Fd656UkkEmseZXPZW0");
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  Firebase.setwriteSizeLimit(fbdo, "tiny");

  timeClient.begin();
  timeClient.setTimeOffset(25200);

  dht.begin();
}


void loop() {
  
  // Perform tasks at regular intervals
  controlWeather();

}




void controlWeather() {
  bool tempFlag1 = false; // Flag for temperature conditions
  bool tempFlag2 = false; // Flag for temperature conditions
  bool humidityFlag = false; // Flag for humidity conditions

  int underThreshold = 25; // Initial temperature threshold
  int upperThreshold = 35; // Initial temperature threshold
  int humidityUnderThreshold = 50;

  float humidity = dht.readHumidity();
  float celciusTemp = dht.readTemperature();


  Firebase.setInt(fbdo, "/Chuong1/temperature/temp", celciusTemp);
  Firebase.setInt(fbdo, "/Chuong1/humidity/humid", humidity);


  if (Firebase.getInt(fbdo, "/Chuong1/temperature/thresholds/underThreshold")) {
    underThreshold = fbdo.intData();
  }

  if (Firebase.getInt(fbdo, "/Chuong1/temperature/thresholds/upperThreshold")) {
    upperThreshold = fbdo.intData();
  }

  if (Firebase.getInt(fbdo, "/Chuong1/humidity/thresholds/underThresholdHumid")) {
    humidityUnderThreshold = fbdo.intData();
  }


  // Check temperature conditions
  if (celciusTemp < underThreshold) {
    tempFlag1 = true; // Temperature is below lower threshold
    tempFlag2 = false;
  } else if (celciusTemp > upperThreshold) {
    tempFlag2 = true;
    tempFlag1 = false; // Temperature is below lower threshold
  } else {
    tempFlag1 = false; // Temperature is below lower threshold
    tempFlag2 = false;
  }

  // Check humidity conditions
  if (humidity < humidityUnderThreshold) {
    humidityFlag = true; // Humidity is below threshold
  } else {
    humidityFlag = false; // Normal humidity
  }

  // Sending temperature character based on flag
  if (tempFlag1) {
    digitalWrite(maysuoi, HIGH);
    //Serial2.write('A'); // Send 'A' character for low temperature
  } else if (tempFlag2) {
    digitalWrite(quattannhiet, HIGH);
    //Serial2.write('B'); // Send 'B' character for high temperature
  } else {
    digitalWrite(maysuoi, LOW);
    digitalWrite(quattannhiet, LOW);

    //Serial2.write('a'); // Send 'a' character for normal temperature
  }

  // Sending humidity character based on flag
  if (humidityFlag) {
    digitalWrite(phunsuong, HIGH);
    //Serial2.write('C'); // Send 'C' character for low humidity
  } else {
    digitalWrite(phunsuong, LOW);
    // Serial2.write('c'); // Send 'c' character for normal humidity
  }
}




//...................................................................
void hengio_dentrangtrai()
{
  if(Firebase.getString(fbdo, "/Chuong1/Hengio/Tatden")) tatdenValue = fbdo.intData();
  Serial.print("Received Tatden value: ");
  Serial.println(tatdenValue);
  if (tatdenValue == 1) {
    enableLed  = false;
  }
 
  ts1 = millis();    
  if(ts1 - t1 > 1000)
  {
    if(Firebase.getInt(fbdo, "/Chuong1/Hengio/confirm"))
    {
      GT = fbdo.intData();
      if(GT != GTS)
      {
        Serial.println("Co thay doi gio cai dat");
        GTS = GT;
        if(Firebase.getInt(fbdo, "/Chuong1/Hengio/Hour")) giocd = fbdo.intData();
        if(Firebase.getInt(fbdo, "/Chuong1/Hengio/Minute")) phutcd = fbdo.intData();
        if(Firebase.getInt(fbdo, "/Chuong1/Hengio/Second")) giaycd = fbdo.intData();
        t1 = ts1;
      }
    }

    //Serial.print(giocd);
    //Serial.print(" -- ");
    //Serial.print(phutcd);  
    //Serial.println(); 
  }

  if(gio == giocd)
  {
    if(phut == phutcd)
    {
      if(giay == giaycd && k == 0)
        {
          enableLed  = true;
          k = 1;
        }
      if(giay != giaycd) k = 0;
    }
  }

  if (enableLed  == true)
  {
    //Serial.print("on ");
    digitalWrite(led, HIGH);
  }
  else
  {
    Serial.print("off ");
    digitalWrite(led, LOW);
  }
}

void hengio_an()
{
  if(Firebase.getString(fbdo, "/Chuong1/Hengio_an/Off")) tatanValue = fbdo.intData();
  Serial.print("Received Tatden value: ");
  Serial.println(tatdenValue);
  if (tatanValue == 1) {
    enableAn = false;
  }
  
  ts2 = millis();

  if(ts2 - t2 > 1000)
  {
    if(Firebase.getInt(fbdo, "/Chuong1/Hengio_an/confirm"))
    {
      GT2 = fbdo.intData();
      if(GT2 != GTS2)
      {
        //Serial.println("Co thay doi gio cai dat");
        GTS2 = GT2;
        if(Firebase.getInt(fbdo, "/Chuong1/Hengio_an/Hour")) giocd2 = fbdo.intData();
        if(Firebase.getInt(fbdo, "/Chuong1/Hengio_an/Minute")) phutcd2 = fbdo.intData();
        if(Firebase.getInt(fbdo, "/Chuong1/Hengio_an/Second")) giaycd2 = fbdo.intData();
        t2 = ts2;
      }
    }
  }

  if(gio == giocd2)
  {
    if(phut == phutcd2)
    {
      if(giay == giaycd2 && k2 == 0)
        {
          enableAn  = true;
          k2 = 1;
        }
      if(giay != giaycd2) k2 = 0;
    }
  }

  if (enableAn == true)
  {
    //Serial.print("on ");
    digitalWrite(led_an, HIGH);
  }
  else
  {
    //Serial.print("off ");
    digitalWrite(led_an, LOW);
  }
}