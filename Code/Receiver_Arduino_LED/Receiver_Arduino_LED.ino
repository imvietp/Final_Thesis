#define RXp2 15
#define TXp2 14
#define LED_PIN 7

void setup() {
  Serial.begin(9600);
  Serial3.begin(115200); // Hardware Serial of Arduino mega
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (Serial3.available() > 0) {
    char message = Serial3.read();
    Serial.println(message); // Debugging print
    
    if (message == 'A') {
      Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(LED_PIN, HIGH); // Turn on LED
    }
    else if (message == 'a') {
      Serial.println("Turning LED off..."); // Debugging print
      digitalWrite(LED_PIN, LOW); // Turn off LED
    }
  }
}
