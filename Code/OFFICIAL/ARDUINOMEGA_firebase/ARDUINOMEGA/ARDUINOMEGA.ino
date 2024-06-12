#include <SoftwareSerial.h>

#define RXp2 15
#define TXp2 14
#define pinMaySuoi 3
#define pinQuatDHT11 4
#define pinPhunSuong 5
#define pinFlameSensor 22
#define pinBuzzer 6
#define pinDapLua 7
#define pinLedTrangTrai 8
#define pinChoAn 9
const int pinMQ2 = A1; // MQ2 sensor analog pin
const int pinBuzzer_MQ2 = 11; // Buzzer pin
const int pinFan_MQ2 = 12; // Fan pin


unsigned long fireDetectedTime = 0; // Variable to store the time when fire was detected
unsigned long pumpStartTime = 0;     // Variable to store the time when pump was turned on
unsigned long lastFlameSensor_CheckTime = 0;


// MQ2 sensor
unsigned long previousMillis = 0;
unsigned long smokeDetectedTime = 0;
unsigned long fanStartTime = 0;
unsigned long lastMQ2Sensor_CheckTime = 0;
const int threshold = 300; // Threshold value for fire detection


//Alarm reciever's phone number with country code
const String PHONE_1 = "+84902946721";
const String PHONE_2 = ""; //optional
const String PHONE_3 = ""; //optional
bool fire_flag = 0;
bool fire_flag_mq2 = 0;

SoftwareSerial sim800L(17, 16);

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200); // Hardware Serial of Arduino mega
  sim800L.begin(9600);
  pinMode(pinFlameSensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  pinMode(pinMaySuoi, OUTPUT);
  pinMode(pinQuatDHT11, OUTPUT);
  pinMode(pinPhunSuong, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinDapLua, OUTPUT);
  pinMode(pinLedTrangTrai, OUTPUT);
  pinMode(pinChoAn, OUTPUT);
  pinMode(pinMQ2, INPUT);
  pinMode(pinBuzzer_MQ2, OUTPUT);
  pinMode(pinFan_MQ2, OUTPUT);

  //----------------------------------------------------------------------------------
  Serial.println("Initializing...");
  //Once the handshake test is successful, it will back to OK
  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1");
  delay(1000);
  //-----------------------------------------------------------------------------------
}


void loop() {
  unsigned long currentTime = millis();
  onoffLED();
  
  if (currentTime - lastFlameSensor_CheckTime  >= 1000) {
    //Serial.println("Performing threshold check...");
    flamesensorFunction();
    lastFlameSensor_CheckTime  = currentTime;
  }

  if (currentTime - lastMQ2Sensor_CheckTime  >= 1000) {
    //Serial.println("Performing threshold check...");
    mq2sensorFunction(); 
    lastMQ2Sensor_CheckTime  = currentTime;
  }
  
} 


void onoffLED()
{
  if (Serial3.available() > 0) {
    char message = Serial3.read();
    //Serial.println(message); // Debugging print
    if (message == 'A') {
      //Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(pinMaySuoi, HIGH); // Turn on LED
    }
    else if (message == 'B') {
      //Serial.println("Turning LED off..."); // Debugging print
      digitalWrite(pinQuatDHT11, HIGH); // Turn off LED
    }

    if (message == 'a') {
      //Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(pinMaySuoi, LOW); // Turn on LED
      digitalWrite(pinQuatDHT11, LOW); // Turn off LED
    }

    if (message == 'C') {
      //Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(pinPhunSuong, HIGH); // Turn on LED
    }
    else if (message == 'c')
    {
      digitalWrite(pinPhunSuong, LOW); // Turn on LED      
    }

    if (message == 'D') {
      //Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(pinLedTrangTrai, HIGH); // Turn on LED
    }
    else if (message == 'd')
    {
      digitalWrite(pinLedTrangTrai, LOW); // Turn on LED      
    }

    if (message == 'E') {
      //Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(pinChoAn, HIGH); // Turn on LED
    }
    else if (message == 'e')
    {
      digitalWrite(pinChoAn, LOW); // Turn on LED      
    }
  }
}


void flamesensorFunction() {
  unsigned long currentMillis = millis(); // Get the current time
  
  // Read from the SIM800L module
  while (sim800L.available()) {
    Serial.println(sim800L.readString());
  }

  int flameValue = digitalRead(pinFlameSensor);

  if (flameValue == 0) { // Fire detected
    digitalWrite(pinBuzzer, HIGH); // Buzzer ON
    if (fireDetectedTime == 0 && fire_flag == 0) {
      send_fire_sms();
      fireDetectedTime = currentMillis; // Store the current time when fire is detected
      fire_flag = 1;
    }
  } 
  else { // No fire detected
    digitalWrite(pinBuzzer, LOW); // Buzzer OFF
    fireDetectedTime = 0; // Reset fire detection time
    fire_flag = 0;
    if (pumpStartTime > 0) {
      digitalWrite(pinDapLua, LOW); // Turn off the pump
      send_fire_off_sms(); // Send "Pump is turned off" message
      pumpStartTime = 0; // Reset pump start time
    }
  }

  // If fire was detected and it's been 3 seconds, turn on the pump
  if (fireDetectedTime > 0 && currentMillis - fireDetectedTime >= 3000 && fire_flag == 1) {
    digitalWrite(pinDapLua, HIGH); // Turn on the pump
    send_fire_destruction_sms(); // Send "Fire is being destroyed" message
    pumpStartTime = currentMillis; // Store the time when pump was turned on
    fire_flag = 2; // Update fire_flag to indicate the pump has been turned on
  }

  // If the pump is on and fire is no longer detected, turn off the pump
  if (pumpStartTime > 0 && flameValue != 0) {
    digitalWrite(pinDapLua, LOW); // Turn off the pump
    send_fire_off_sms(); // Send "Pump is turned off" message
    pumpStartTime = 0; // Reset pump start time
    fire_flag = 0; // Reset fire flag
  }
}

void mq2sensorFunction() {
  unsigned long currentMillis = millis(); // Get the current time

  int sensorValue = readSensor();

  if (sensorValue >= threshold) { // Value exceeds threshold
    digitalWrite(pinBuzzer_MQ2, HIGH); // Buzzer ON
    if (smokeDetectedTime == 0 && fire_flag_mq2 == 0) {
      send_mq2_sms();
      smokeDetectedTime = currentMillis; // Store the current time when the value exceeds the threshold
      fire_flag_mq2 = 1;
    }
  } else { // Value below threshold
    digitalWrite(pinBuzzer_MQ2, LOW); // Buzzer OFF
    smokeDetectedTime = 0; // Reset detection time
    fire_flag_mq2 = 0;
    if (fanStartTime > 0) {
      digitalWrite(pinFan_MQ2, LOW); // Turn off the fan
      send_fan_off_sms(); // Send "Fan is turned off and safe" message
      fanStartTime = 0; // Reset fan start time
    }
  }

  // If value was above threshold and it's been 3 seconds, turn on the fan
  if (smokeDetectedTime > 0 && currentMillis - smokeDetectedTime >= 4000 && fire_flag_mq2 == 1) {
    digitalWrite(pinFan_MQ2, HIGH); // Turn on the fan
    send_fan_on_sms(); // Send "Fan is on" message
    fanStartTime = currentMillis; // Store the time when the fan was turned on
    fire_flag_mq2 = 2; // Update fire_flag to indicate the fan has been turned on
  }

  // If the fan is on and the value is below threshold, turn off the fan
  if (fanStartTime > 0 && sensorValue < threshold) {
    digitalWrite(pinFan_MQ2, LOW); // Turn off the fan
    send_fan_off_sms(); // Send "Fan is turned off and safe" message
    fanStartTime = 0; // Reset fan start time
    fire_flag_mq2 = 0; // Reset fire flag
  }
}


int readSensor() {
  unsigned long currentMillis = millis(); // Get the current time in milliseconds

  // Check if 1000 milliseconds have passed since the last reading
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis; // Update the last reading time

    unsigned int sensorValue = analogRead(pinMQ2);  // Read the analog value from the sensor
    unsigned long currentTime = millis();           // Get the current time in milliseconds

    Serial.print("Time: ");
    Serial.print(currentTime);
    Serial.print(" ms, Analog output: ");
    Serial.println(sensorValue);

    return sensorValue; // Return the analog sensor value
  }
  return -1; // Return -1 if 1000 milliseconds have not passed
}

////////////////////////////////////////////////// MQ2 SENSOR /////////////////////////////////////////////
void send_mq2_sms() {
  if (PHONE_1 != "") {
    Serial.print("Phone 1: ");
    send_sms("MQ2 is Detected", PHONE_1);
  }
  if (PHONE_2 != "") {
    Serial.print("Phone 2: ");
    send_sms("MQ2 is Detected", PHONE_2);
  }
  if (PHONE_3 != "") {
    Serial.print("Phone 3: ");
    send_sms("MQ2 is Detected", PHONE_3);
  }
}


void send_fan_on_sms() {
  if (PHONE_1 != "") {
    send_sms("MQ2 is being destroyed", PHONE_1);
  }
  if (PHONE_2 != "") {
    send_sms("MQ2 is being destroyed", PHONE_2);
  }
  if (PHONE_3 != "") {
    send_sms("MQ2 is being destroyed", PHONE_3);
  }
}

void send_fan_off_sms() {
  if (PHONE_1 != "") {
    send_sms("Fan is off", PHONE_1);
  }
  if (PHONE_2 != "") {
    send_sms("Fan is of", PHONE_2);
  }
  if (PHONE_3 != "") {
    send_sms("Fan is of", PHONE_3);
  }
}


////////////////////////////////////// FLAME SENSOR ///////////////////////////////////////////////
void send_fire_sms() {
  if (PHONE_1 != "") {
    Serial.print("Phone 1: ");
    send_sms("Fire is Detected", PHONE_1);
  }
  if (PHONE_2 != "") {
    Serial.print("Phone 2: ");
    send_sms("Fire is Detected", PHONE_2);
  }
  if (PHONE_3 != "") {
    Serial.print("Phone 3: ");
    send_sms("Fire is Detected", PHONE_3);
  }
}


void send_fire_destruction_sms() {
  if (PHONE_1 != "") {
    send_sms("Fire is being destroyed", PHONE_1);
  }
  if (PHONE_2 != "") {
    send_sms("Fire is being destroyed", PHONE_2);
  }
  if (PHONE_3 != "") {
    send_sms("Fire is being destroyed", PHONE_3);
  }
}

void send_fire_off_sms() {
  if (PHONE_1 != "") {
    send_sms("Pump is off", PHONE_1);
  }
  if (PHONE_2 != "") {
    send_sms("Pump is off", PHONE_2);
  }
  if (PHONE_3 != "") {
    send_sms("Pump is off", PHONE_3);
  }
}

/////////////////////////////// SEND SMS FUNCTION /////////////////////////////////////
void send_sms(String text, String phone) {
  Serial.println("sending sms....");
  delay(50);
  sim800L.print("AT+CMGF=1\r");
  delay(1000);
  sim800L.print("AT+CMGS=\"" + phone + "\"\r");
  delay(1000);
  sim800L.print(text);
  delay(100);
  sim800L.write(0x1A); // ASCII code for CTRL+Z
  delay(5000);
}

void make_call(String phone) {
  Serial.println("calling....");
  sim800L.println("ATD" + phone + ";");
  delay(20000); // 20 sec delay
  sim800L.println("ATH");
  delay(1000); // 1 sec delay
}
