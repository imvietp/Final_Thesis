#define RXp2 15
#define TXp2 14
#define LED_PIN1 3
#define LED_PIN2 4
#define flame_sensor 22
#define LED_PIN_flame 5



void setup() {
  Serial.begin(9600);
  Serial3.begin(115200); // Hardware Serial of Arduino mega
  pinMode(flame_sensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN_flame, OUTPUT);

}



void loop() {
  delay(1000);
  onoffLED();
  flamesensorFunction();
}


void onoffLED()
{
  if (Serial3.available() > 0) {
    char message = Serial3.read();
    Serial.println(message); // Debugging print
    
    if (message == 'A') {
      Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(LED_PIN1, HIGH); // Turn on LED
    }
    else if (message == 'a') {
      Serial.println("Turning LED off..."); // Debugging print
      digitalWrite(LED_PIN1, LOW); // Turn off LED
    }

    if (message == 'B') {
      Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(LED_PIN2, HIGH); // Turn on LED
    }
    else if (message == 'b') {
      Serial.println("Turning LED off..."); // Debugging print
      digitalWrite(LED_PIN2, LOW); // Turn off LED
    }
  }
}



void flamesensorFunction()
{
    if (digitalRead(flame_sensor) == 0 )
  {
    digitalWrite(LED_PIN_flame, HIGH); // Led ON
    Serial.println("** Fire detected!!! **");
  }
  else
  {
    digitalWrite(LED_PIN_flame, LOW); // Led OFF
    Serial.println("No Fire detected");
  }
}