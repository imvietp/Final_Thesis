
#define RXp2 15
#define TXp2 14
#define BUTTON_PIN 5

void setup() {
  Serial.begin(9600);
  Serial3.begin(115200); // Hardware Serial of Arduino mega

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == LOW) { // Button pressed
    Serial.println("Button pressed");
    Serial3.write('a'); // Send 'a' character
    delay(200); // Debounce delay
  }
  else {
    Serial.println("Button not pressed");
    Serial3.write('A'); // Send 'A' character
  }
}
