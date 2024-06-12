int light_sensor = 15; 
int led_light_sensor = 18;
 
void setup() {
Serial.begin(115200); //begin Serial Communication
}
 
void loop() {
  int raw_light = analogRead(light_sensor); // read the raw value from light_sensor pin (A3)
  int light = map(raw_light, 0, 1023, 0, 100); // map the value from 0, 1023 to 0, 100
 
  Serial.print("Light level: "); 
  Serial.println(light); // print the light value in Serial Monitor

  if(light >= 300)
  {
    analogWrite(led_light_sensor, light);
  }
  else
  {
    analogWrite(led_light_sensor, LOW);
  }

 
  delay(1000); // add a delay to only read and print every 1 second
}
