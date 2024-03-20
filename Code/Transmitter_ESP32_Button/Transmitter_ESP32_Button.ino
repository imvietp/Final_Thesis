#define RXp2 16
#define TXp2 17
#define BUTTON_PIN 5

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); // Hardware Serial of ESP32

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == LOW) { // Button pressed
    Serial.println("Button pressed");
    Serial2.write('a'); // Send 'a' character
    delay(200); // Debounce delay
  }
  else {
    Serial.println("Button not pressed");
    Serial2.write('A'); // Send 'A' character
  }
}
