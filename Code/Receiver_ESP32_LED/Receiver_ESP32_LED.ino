#define RXp2 17
#define TXp2 16
#define LED_PIN 7
void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}


void loop() {
  if (Serial2.available() > 0) {
    String message = Serial2.readStringUntil('\n');
    Serial.println(message); // Debugging print
    if (message == "on") {
      Serial.println("Turning LED on..."); // Debugging print
      digitalWrite(7, HIGH); // Turn on LED
      //delay(1000); // LED on time
      //Serial.println("Turning LED off..."); // Debugging print
     // digitalWrite(LED_PIN, LOW); // Turn off LED
    }
  }
}
