#define flame_sensor 5
#define led 18

#define mq2_sensor 34
#define led_mq2 19

int alarm_buzzer = 22;

unsigned long previousMillis = 0;
const long interval = 1000; // Interval in milliseconds for reading sensors


void setup()
{
  pinMode(flame_sensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  pinMode(led, OUTPUT);// initialize digital pin LED_BUILTIN as an output.

  pinMode(led_mq2, OUTPUT);
  analogWrite(led_mq2, LOW);

  pinMode(alarm_buzzer, OUTPUT);
  digitalWrite(alarm_buzzer, HIGH);
  Serial.begin(112500);// initialize serial communication @ 9600 baud:
}

// MAIN function
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    flame_sensor_function();
    print_sensor_MQ2_function();
    Serial.println("\n");
  }
}


void flame_sensor_function()
{
  if (digitalRead(flame_sensor) == 0 )
  {
    digitalWrite(led, HIGH); // Led ON
    Serial.println("** Fire detected!!! **");
  }
  else
  {
    digitalWrite(led, LOW); // Led OFF
    Serial.println("No Fire detected");
  }
}


// MQ2 SENSOR FUNCTION
// Read sensor
int readSensor_MQ2_function() {
  unsigned int sensorValue = analogRead(mq2_sensor);
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255);
  if (outputValue > 400) {
    analogWrite(led_mq2, outputValue);
    noTone(alarm_buzzer);
  }
  else {
    analogWrite(led_mq2, LOW);
    noTone(alarm_buzzer);
  }
  return outputValue;
}


// Print the value to terminal
void print_sensor_MQ2_function() {
  Serial.print("Analog MQ2 output: ");
  Serial.println(readSensor_MQ2_function());
}
