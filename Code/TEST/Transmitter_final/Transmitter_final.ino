#define RXp2 16
#define TXp2 17
#define light_sensor 15
#define flame_sensor 5


void setup() {
  pinMode(flame_sensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); // Hardware Serial of ESP32
}

void loop() {
  light_sensor_function();
  flame_sensor_function();
  delay(1000);
}


void light_sensor_function() {
  int raw_light = analogRead(light_sensor);
  int light = map(raw_light, 0, 1023, 0, 100);
  Serial.print("Light output: ");
  Serial.println(light);
  if (light >= 100) {
    Serial2.write('A'); // Send 'A' character
  }
  else {
    Serial2.write('a'); // Send 'a' character
  }
}


void flame_sensor_function()
{
  int flame_pin = digitalRead(flame_sensor);          // reading from the sensor
  if (flame_pin == LOW)                               // applying condition
  {
    Serial.println("FLAME, FLAME, FLAME");
    Serial2.write('B');
    //digitalWrite(led_pin, HIGH);                      // if state is high, then turn high the led
  
  }
  else
  {
    Serial.println("no flame");
    Serial2.write('b');
    //digitalWrite(led_pin, LOW);                       // otherwise turn it low
  }
}