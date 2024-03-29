#define flame_sensor 5
#define led 27


void setup()
{
  pinMode(flame_sensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  pinMode(led, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
  Serial.begin(112500);// initialize serial communication @ 9600 baud:
}
void loop()
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
  delay(100);
}