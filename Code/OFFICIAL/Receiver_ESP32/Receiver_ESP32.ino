#define RXp2 16
#define TXp2 17

#define LED_PIN 27

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); // Hardware Serial of ESP32
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (Serial2.available() > 0) {
    char message = Serial2.read();
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
