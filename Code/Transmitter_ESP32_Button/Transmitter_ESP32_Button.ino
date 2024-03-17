#define RXp2 16
#define TXp2 17
#define BUTTON_PIN 5

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == 0) { // Button pressed
    Serial2.println("on");
    delay(1000); // Debounce delay
  }
}
