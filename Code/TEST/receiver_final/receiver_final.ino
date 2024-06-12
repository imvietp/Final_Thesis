#define RXp2 15
#define TXp2 14
#define light_sensor 15
#define led_light_sensor 3
#define led_flame_sensor 4


void setup() {
  Serial.begin(9600);
  Serial3.begin(115200); // Hardware Serial of Arduino Mega
  pinMode(led_light_sensor, OUTPUT);
  pinMode(led_flame_sensor, OUTPUT);
  //pinMode(led_mq2, OUTPUT);
}

void loop()
{
  if (Serial3.available() > 0) {
    light_sensor_function();
    flame_sensor_function();
  }
}



void light_sensor_function() {

    char message_light = Serial3.read();

    int raw_light = analogRead(light_sensor);
    int light = map(raw_light, 0, 1023, 0, 100);

    if (message_light == 'A') {
      analogWrite(led_light_sensor, light);
    }

    else if (message_light == 'a') {
      analogWrite(led_light_sensor, LOW);
    }   
  
}

void flame_sensor_function() {

    char message_flame = Serial3.read();

    if (message_flame == 'B') {
      digitalWrite(led_flame_sensor, HIGH);
    }

    else if (message_flame == 'b') {
      digitalWrite(led_flame_sensor, LOW);
    }   
  
}


