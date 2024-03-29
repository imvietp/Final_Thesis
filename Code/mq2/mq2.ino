// Sensor pins pin D6 LED output, pin A0 analog Input

#define ledPin 26

#define sensorPin 25



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


  if (outputValue > 400)
  {
    analogWrite(ledPin, outputValue); // generate PWM signal
  }
  else
  {
    analogWrite(ledPin, LOW);
  }

  return outputValue;             // Return analog moisture value

}

