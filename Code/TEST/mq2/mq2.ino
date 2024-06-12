// Sensor pins pin D6 LED output, pin A0 analog Input

#define ledPin 3

#define sensorPin A1

bool flag = 0;
unsigned long lastMQ2CheckTime = 0;

void setup() {

  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

}


void loop() {

  Serial.print("Analog output: ");

  Serial.println(readSensor());

  delay(500);

}


//  This function returns the analog data to calling function

int readSensor() {

  unsigned int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor

  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255); // map the 10-bit data to 8-bit data


  if (sensorValue >= 200 && flag == 0 )
  {
    analogWrite(ledPin, sensorValue); // generate PWM signal
    flag = 1;
  }
  else if (sensorValue < 200 && flag == 1)
  {
    analogWrite(ledPin, LOW);
    flag = 0;
  }

  return sensorValue;             // Return analog moisture value

}

