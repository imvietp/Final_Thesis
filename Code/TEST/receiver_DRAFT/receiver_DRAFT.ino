#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define RXp2 15
#define TXp2 14


#define pinMaySuoi 5
#define pinQuatDHT11 6
#define pinPhunSuong 7


#define pinBuzzerFlameSensor 3
#define pinBuzzer_MQ2  4


#define pinMQ2Sensor  A1
#define pinFlameSensor 2


#define pinFanMQ2  12
#define pinPhunSuongDapLua 11


#define pinLedTrangTrai 8
#define pinChoAn 9
#define pinVeSinh A0


#define buttonDenSuoi 31
#define buttonQuatDHT11 33
#define buttonPhunSuong 35
#define buttonDenTrangTrai 37
#define buttonChoHeoAn 39
#define buttonVeSinh 41


bool tempDenSuoi = LOW;
bool tempQuatDHT11 = LOW;
bool tempPhunSuong = LOW;
bool tempQuatHut = LOW;
bool tempDenTrangTrai = LOW;
bool tempChoHeoAn = LOW;
bool tempVeSinh = LOW;


unsigned long fireDetectedTime = 0; 
unsigned long pumpStartTime = 0; 
unsigned long lastFlameSensor_CheckTime = 0;


unsigned long previousMillis = 0;
unsigned long smokeDetectedTime = 0;
unsigned long fanStartTime = 0;
unsigned long lastMQ2Sensor_CheckTime = 0;
const int threshold = 500; // Threshold value for fire detection
bool phoneNumberMode = false; // Flag to indicate whether the system is in phone number reception mode


String phoneNumber = "";
String receivedNumber = "";


// EEPROM address to store the phone number
const int EEPROM_ADDRESS = 0;
const int PHONE_NUMBER_MAX_LENGTH = 20; // Adjust according to the expected length of the phone number


bool fire_flag = 0;
bool fire_flag_mq2 = 0;


SoftwareSerial sim800L(17, 16);
LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD I2C address to 0x27 for a 20 chars and 4 line display

const int numDevices = 10;
String deviceStatuses[numDevices] = {
  "DenSuoi: OFF",
  "Quat: OFF",
  "P_Suong: OFF",
  "FarmLED: OFF",
  "Feed: OFF",
  "Clean: OFF"
};


void setup() {
  Serial.begin(115200);
  Serial3.begin(115200); // Hardware Serial of Arduino mega
  sim800L.begin(9600);
  pinMode(buttonDenSuoi, INPUT_PULLUP);
  pinMode(buttonQuatDHT11, INPUT_PULLUP);
  pinMode(buttonPhunSuong, INPUT_PULLUP);
  pinMode(buttonDenTrangTrai, INPUT_PULLUP);
  pinMode(buttonChoHeoAn, INPUT_PULLUP);
  pinMode(buttonVeSinh, INPUT_PULLUP);
  pinMode(pinFlameSensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  pinMode(pinMaySuoi, OUTPUT);
  pinMode(pinQuatDHT11, OUTPUT);
  pinMode(pinPhunSuong, OUTPUT);
  pinMode(pinBuzzerFlameSensor, OUTPUT);
  pinMode(pinPhunSuongDapLua, OUTPUT);
  pinMode(pinLedTrangTrai, OUTPUT);
  pinMode(pinChoAn, OUTPUT);
  pinMode(pinMQ2Sensor, INPUT);
  pinMode(pinBuzzer_MQ2, OUTPUT);
  pinMode(pinFanMQ2, OUTPUT);
  pinMode(pinVeSinh, OUTPUT);

  Serial.println("Initializing...");

  lcd.init();
  lcd.backlight();

  //Once the handshake test is successful, it will back to OK
  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1");
  delay(1000);

  // Retrieve phone number from EEPROM
  retrievePhoneNumber();

  lcd.setCursor(10, 2);
  lcd.print("Pump: Off"); // Initial pump status

  lcd.setCursor(10, 3);
  lcd.print("TG: Off");
  updateLCD();
}
 



void loop() {
  unsigned long currentTime = millis();
  
  if (Serial3.available() > 0) 
  {
      char command = Serial3.peek(); // Peek the next character without removing it from the buffer
      if (command == '+') 
      {
          receivePhoneNumber(); // Handle phone number reception
      } else {
          onoffLED(); // Handle control command
      }
  }

  if (currentTime - lastFlameSensor_CheckTime >= 200) 
  {
    flamesensorFunction();
    
    lastFlameSensor_CheckTime = currentTime;
  }

  if (currentTime - lastMQ2Sensor_CheckTime >= 1000) 
  {
    mq2sensorFunction();
    lastMQ2Sensor_CheckTime = currentTime;
  }
  manualOnOff();
}



////////////////////////// ON OFF LED /////////////////////////////////
void onoffLED() {
  if (Serial3.available() > 0) {
    char message = Serial3.read();
    switch (message) {
      case 'A':
        digitalWrite(pinMaySuoi, HIGH); // Turn on heater
        deviceStatuses[0] = "DS: ON";
        tempDenSuoi = true;
        break;
      case 'B':
        digitalWrite(pinQuatDHT11, HIGH); // Turn on fan
        deviceStatuses[1] = "Quat: ON";
        tempQuatDHT11 = true;
        break;
      case 'a':
        digitalWrite(pinMaySuoi, LOW); // Turn off heater
        digitalWrite(pinQuatDHT11, LOW); // Turn off fan
        deviceStatuses[0] = "DS: OFF";
        deviceStatuses[1] = "Quat: OFF";
        tempDenSuoi = false;
        tempQuatDHT11 = false;
        break;
      case 'C':
        digitalWrite(pinPhunSuong, HIGH); // Turn on misting
        deviceStatuses[2] = "PS: ON";
        tempPhunSuong = true;
        break;
      case 'G':
        digitalWrite(pinFanMQ2, HIGH); // Turn on misting
        tempQuatHut = true;
        break;
      case 'c':
        digitalWrite(pinPhunSuong, LOW); // Turn off misting
        digitalWrite(pinFanMQ2, LOW); // Turn on misting
        deviceStatuses[2] = "PS: OFF";
        tempPhunSuong = false;
        tempQuatHut = false;
        break;
      case 'D':
        digitalWrite(pinLedTrangTrai, HIGH); // Turn on farm LED
        deviceStatuses[3] = "Den: ON";
        tempDenTrangTrai = true;
        break;
      case 'd':
        digitalWrite(pinLedTrangTrai, LOW); // Turn off farm LED
        deviceStatuses[3] = "Den: OFF";
        tempDenTrangTrai = false;
        break;
      case 'E':
        digitalWrite(pinChoAn, HIGH); // Turn on feeding system
        lcd.setCursor(14, 0);
        lcd.print("EAT: ON");
        tempChoHeoAn = true;
        break;
      case 'e':
        digitalWrite(pinChoAn, LOW); // Turn off feeding system
        lcd.setCursor(14, 0);
        lcd.print("EAT: OFF");
        tempChoHeoAn = false;
        break;
      case 'F':
        digitalWrite(pinVeSinh, HIGH); // Turn on cleaning system
        lcd.setCursor(14, 1);
        lcd.print("VS: ON");
        tempVeSinh = true;
        break;
      case 'f':
        digitalWrite(pinVeSinh, LOW); // Turn off cleaning system
        lcd.setCursor(14, 1);
        lcd.print("VS: OFF");
        tempVeSinh = false;
        break;
      default:
        Serial.println("Received unrecognized command: " + String(message)); // Debugging print
        return;
    }
    updateLCD(); // Update the LCD with the new statuses
  }
}

void manualOnOff() {
  if (digitalRead(buttonDenSuoi) == LOW) {
    delay(10);
    if (digitalRead(buttonDenSuoi) == LOW) {
      tempDenSuoi = !tempDenSuoi;
      digitalWrite(pinMaySuoi, tempDenSuoi);
      updateLCD();
    }
    while (digitalRead(buttonDenSuoi) == LOW);
  } 
  else if (digitalRead(buttonQuatDHT11) == LOW) {
    delay(10);
    if (digitalRead(buttonQuatDHT11) == LOW) {
      tempQuatDHT11 = !tempQuatDHT11;
      digitalWrite(pinQuatDHT11, tempQuatDHT11);
      updateLCD();
    }
    while (digitalRead(buttonQuatDHT11) == LOW);
  } 
  else if (digitalRead(buttonPhunSuong) == LOW) {
    delay(10);
    if (digitalRead(buttonPhunSuong) == LOW) {
      tempPhunSuong = !tempPhunSuong;
      digitalWrite(pinPhunSuong, tempPhunSuong);
      updateLCD();
    }
    while (digitalRead(buttonPhunSuong) == LOW);
  } 
  else if (digitalRead(buttonDenTrangTrai) == LOW) {
    delay(10);
    if (digitalRead(buttonDenTrangTrai) == LOW) {
      tempDenTrangTrai = !tempDenTrangTrai;
      digitalWrite(pinLedTrangTrai, tempDenTrangTrai);
      updateLCD();
    }
    while (digitalRead(buttonDenTrangTrai) == LOW);
  } 
  else if (digitalRead(buttonChoHeoAn) == LOW) {
    delay(10);
    if (digitalRead(buttonChoHeoAn) == LOW) {
      tempChoHeoAn = !tempChoHeoAn;
      digitalWrite(pinChoAn, tempChoHeoAn);
      updateLCD();
    }
    while (digitalRead(buttonChoHeoAn) == LOW);
  } 
  else if (digitalRead(buttonVeSinh) == LOW) {
    delay(10);
    if (digitalRead(buttonVeSinh) == LOW) {
      tempVeSinh = !tempVeSinh;
      digitalWrite(pinVeSinh, tempVeSinh);
      updateLCD();
    }
    while (digitalRead(buttonVeSinh) == LOW);
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DS: ");
  lcd.print(tempDenSuoi ? "On" : "Off");

  lcd.setCursor(0, 1);
  lcd.print("Quat: ");
  lcd.print(tempQuatDHT11 ? "On" : "Off");

  lcd.setCursor(0, 2);
  lcd.print("PS: ");
  lcd.print(tempPhunSuong ? "On" : "Off");

  lcd.setCursor(0, 3);
  lcd.print("Den: ");
  lcd.print(tempDenTrangTrai ? "On" : "Off");

  lcd.setCursor(10, 0); // Adjust cursor position for next column
  lcd.print("An: ");
  lcd.print(tempChoHeoAn ? "On" : "Off");

  lcd.setCursor(10, 1); // Adjust cursor position for next column
  lcd.print("VS: ");
  lcd.print(tempVeSinh ? "On" : "Off");

  lcd.setCursor(10, 2);
  lcd.print("DL: Off"); // Initial pump status

  lcd.setCursor(10, 3);
  lcd.print("TG: Off"); // Initial pump status
}

/*
void updateLCD() {
  lcd.clear();
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0, i);
    lcd.print(deviceStatuses[i]);
  }
}
*/

//////////////////////// RECEIVE PHONE NUMBER /////////////////////////////////
void receivePhoneNumber() {
    phoneNumber = Serial3.readStringUntil('\n'); // Read phone number from Serial3
    phoneNumber.trim(); // Remove any trailing newline or space characters
    // Check if the received data is a valid phone number
    if (isValidPhoneNumber(phoneNumber)) {
        storePhoneNumber(phoneNumber); // Store the phone number if valid
        Serial.println("Phone number received and stored: " + phoneNumber); // Debugging print
    } else {
        Serial.println("Received unrecognized data: " + phoneNumber); // Debugging print
    }
}




bool isValidPhoneNumber(const String& number) {
    // A valid phone number starts with + and has digits
    if (number.startsWith("+") && number.length() > 1) {
        for (unsigned int i = 1; i < number.length(); i++) {
            if (!isdigit(number.charAt(i))) {
                return false;
            }
        }
        return true;
    }
    return false;
}




bool isNumeric(String str) 
{
  for (unsigned int i = 0; i < str.length(); i++) 
  {
    if (!isdigit(str.charAt(i))) 
    {
      return false;
    }
  }
  return true;
}




void storePhoneNumber(String phoneNumber) {
  // Check if the phone number is a control character
  if (phoneNumber == "A" || phoneNumber == "B" || phoneNumber == "a" || phoneNumber == "C" || phoneNumber == "c" || phoneNumber == "D" || phoneNumber == "d" || phoneNumber == "E" || phoneNumber == "e") {
    Serial.println("Received a control character. Ignoring."); // Debugging print
    return; // Ignore control characters
  }
  
  int len = phoneNumber.length();
  Serial.println("Storing phone number: " + phoneNumber + " Length: " + String(len)); // Debugging print
  EEPROM.write(EEPROM_ADDRESS, len); // Write the length of the phone number
  for (int i = 0; i < len; i++) {
    EEPROM.write(EEPROM_ADDRESS + 1 + i, phoneNumber[i]); // Write each character of the phone number
    Serial.println("Writing char: " + String(phoneNumber[i]) + " at address: " + String(EEPROM_ADDRESS + 1 + i)); // Debugging print
  }
}


 

void retrievePhoneNumber() {
  int len = EEPROM.read(EEPROM_ADDRESS); // Read the length of the phone number
  Serial.println("Length retrieved from EEPROM: " + String(len)); // Debugging print

  if (len > 0 && len <= PHONE_NUMBER_MAX_LENGTH) {
    char phoneNumberArr[PHONE_NUMBER_MAX_LENGTH + 1]; // Temporary array to hold the phone number
    for (int i = 0; i < len; i++) {
      phoneNumberArr[i] = EEPROM.read(EEPROM_ADDRESS + 1 + i); // Read each character of the phone number
      Serial.println("Reading char: " + String(phoneNumberArr[i]) + " from address: " + String(EEPROM_ADDRESS + 1 + i)); // Debugging print
    }
    phoneNumberArr[len] = '\0'; // Null-terminate the string
    phoneNumber = String(phoneNumberArr); // Convert the character array to a String
    Serial.println("Phone number retrieved from EEPROM: " + phoneNumber); // Debugging print
  } else {
    Serial.println("No valid phone number stored in EEPROM"); // Debugging print
  }
}




/////////////////////////// FLAME SENSOR FUNCTION ////////////////////////////////////////
void flamesensorFunction() {
  unsigned long currentMillis = millis(); // Get the current time

  // Read from the SIM800L module
  while (sim800L.available())
  {
    Serial.println(sim800L.readString());
  }

  int flameValue = digitalRead(pinFlameSensor);

  if (flameValue == 0) 
  { // Fire detected
    digitalWrite(pinBuzzerFlameSensor, HIGH); // Buzzer ON
    if (fireDetectedTime == 0 && fire_flag == 0) 
    {
      send_fire_sms();
      fireDetectedTime = currentMillis; // Store the current time when fire is detected
      fire_flag = 1;
    }
  } 
  else 
  { // No fire detected
    digitalWrite(pinBuzzerFlameSensor, LOW); // Buzzer OFF
    fireDetectedTime = 0; // Reset fire detection time
    fire_flag = 0;
    if (pumpStartTime > 0) 
    {
      digitalWrite(pinPhunSuongDapLua, LOW); // Turn off the pump
      lcd.setCursor(10, 2);
      lcd.print("Pump: Off");
      send_fire_off_sms(); // Send "Pump is turned off" message
      pumpStartTime = 0; // Reset pump start time
    }
  }

  // If fire was detected and it's been 3 seconds, turn on the pump
  if (fireDetectedTime > 0 && currentMillis - fireDetectedTime >= 3000 && fire_flag == 1) 
  {
    digitalWrite(pinPhunSuongDapLua, HIGH); // Turn on the pump
    lcd.setCursor(10, 2);
    lcd.print("Pump: On "); 
    send_fire_destruction_sms(); // Send "Fire is being destroyed" message
    pumpStartTime = currentMillis; // Store the time when pump was turned on
    fire_flag = 2; // Update fire_flag to indicate the pump has been turned on   
  }

  // If the pump is on and fire is no longer detected, turn off the pump
  if (pumpStartTime > 0 && flameValue != 0) 
  {
    digitalWrite(pinPhunSuongDapLua, LOW); // Turn off the pump
    lcd.setCursor(10, 2);
    lcd.print("Pump: Off");   
    send_fire_off_sms(); // Send "Pump is turned off" message
    pumpStartTime = 0; // Reset pump start time
    fire_flag = 0; // Reset fire flag 
  }
}




//////////////////////////////////////////// MQ2 SENSOR ///////////////////////////////////////
void mq2sensorFunction() {
  unsigned long currentMillis = millis(); // Get the current time

  int sensorValue = readSensor();

  if (sensorValue >= threshold) 
  { // Value exceeds threshold
    digitalWrite(pinBuzzer_MQ2, HIGH); // Buzzer ON
    if (smokeDetectedTime == 0 && fire_flag_mq2 == 0) 
    {
      send_mq2_sms();
      smokeDetectedTime = currentMillis; // Store the current time when the value exceeds the threshold
      fire_flag_mq2 = 1;
    }
  } 
  else 
  { // Value below threshold
    digitalWrite(pinBuzzer_MQ2, LOW); // Buzzer OFF
    smokeDetectedTime = 0; // Reset detection time
    fire_flag_mq2 = 0;
    if (fanStartTime > 0) 
    {
      digitalWrite(pinFanMQ2, LOW); // Turn off the fan
      lcd.setCursor(10, 3);
      lcd.print("TG: Off");
      send_fan_off_sms(); // Send "Fan is turned off and safe" message
      fanStartTime = 0; // Reset fan start time
    }
  }

  // If value was above threshold and it's been 3 seconds, turn on the fan
  if (smokeDetectedTime > 0 && currentMillis - smokeDetectedTime >= 2500 && fire_flag_mq2 == 1) 
  {
    digitalWrite(pinFanMQ2, HIGH); // Turn on the fan
    lcd.setCursor(10, 3);
    lcd.print("TG: On ");
    send_fan_on_sms(); // Send "Fan is on" message
    fanStartTime = currentMillis; // Store the time when the fan was turned on
    fire_flag_mq2 = 2; // Update fire_flag to indicate the fan has been turned on
  }

  // If the fan is on and the value is below threshold, turn off the fan
  if (fanStartTime > 0 && sensorValue < threshold) 
  {
    digitalWrite(pinFanMQ2, LOW); // Turn off the fan
    lcd.setCursor(10, 3);
    lcd.print("TG: Off");
    send_fan_off_sms(); // Send "Fan is turned off and safe" message
    fanStartTime = 0; // Reset fan start time
    fire_flag_mq2 = 0; // Reset fire flag
  }
}




int readSensor() 
{
  unsigned long currentMillis = millis(); // Get the current time in milliseconds

  // Check if 1000 milliseconds have passed since the last reading
  if (currentMillis - previousMillis >= 1000) 
  {
    previousMillis = currentMillis; // Update the last reading time

    unsigned int sensorValue = analogRead(pinMQ2Sensor); // Read the analog value from the sensor
    unsigned long currentTime = millis(); // Get the current time in milliseconds

    //Serial.print("Time: ");
    //Serial.print(currentTime);
    //Serial.print(" ms, Analog output: ");
    //Serial.println(sensorValue);

    return sensorValue; // Return the analog sensor value
  }
  return -1; // Return -1 if 1000 milliseconds have not passed
}




void send_mq2_sms() 
{
  if (phoneNumber != "") 
  {
    send_sms("MQ2 is Detected", phoneNumber);
  }
}




void send_fan_on_sms() 
{
  if (phoneNumber != "") 
  {
    send_sms("MQ2 is being destroyed", phoneNumber);
  }
}




void send_fan_off_sms() 
{
  if (phoneNumber != "") 
  {
    send_sms("Fan is off", phoneNumber);
  }
}

////////////////////////////////////// FLAME SENSOR ///////////////////////////////////////////////
void send_fire_sms() 
{
  if (phoneNumber != "") 
  {
    send_sms("Fire is Detected", phoneNumber);
  }
}



void send_fire_destruction_sms() 
{
  if (phoneNumber != "") 
  {
    send_sms("Fire is being destroyed", phoneNumber);
  }
}



void send_fire_off_sms()
{
  if (phoneNumber != "") 
  {
    send_sms("Pump is off", phoneNumber);
  }
}


/////////////////////////////// SEND SMS FUNCTION /////////////////////////////////////
void send_sms(String text, String phone) 
{
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



void make_call(String phone) 
{
  Serial.println("calling....");
  sim800L.println("ATD" + phone + ";");
  delay(20000); // 20 sec delay
  sim800L.println("ATH");
  delay(1000); // 1 sec delay
}
