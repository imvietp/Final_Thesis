#define flame_sensor 2
//#define led 27
//#define RXp2 16
//#define TXp2 17

void setup()
{
  pinMode(flame_sensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  //pinMode(led, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);// initialize serial communication @ 9600 baud:
  //Serial2.begin(250000, SERIAL_8N1, RXp2, TXp2); // Hardware Serial of ESP32
}

void loop()
{
  if (digitalRead(flame_sensor) == 0 )
  {
    Serial2.write('D'); // Send 'a' character for normal temperature
    Serial.println("** Fire detected!!! **");
  }
  
  else
  {
    Serial2.write('d'); // Send 'a' character for normal temperature
    //Serial.println("No Fire detected");
  }

}