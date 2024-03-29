#include <LiquidCrystal.h>
#include <DHT.h>

// Define pins for buttons
#define TEMP_UP_PIN 2
#define TEMP_DOWN_PIN 3

// Define pin for DHT11 sensor
#define DHTPIN 4

// Define type of sensor (DHT11)
#define DHTTYPE DHT11 

// Define pins for buzzer
#define BUZZER_PIN 5

// Define LCD pins
const int rs = 7, en = 6, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define global variables
int min_temperature_threshold = 20;
int max_temperature_threshold = 30;

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Setup pins for buttons
  pinMode(TEMP_UP_PIN, INPUT_PULLUP);
  pinMode(TEMP_DOWN_PIN, INPUT_PULLUP);

  // Setup pin for buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize LCD
  lcd.begin(16, 2);

  // Start DHT sensor
  dht.begin();

  // Display initial temperature thresholds
  updateThresholdDisplay();
}

void loop() {
  // Read temperature from DHT sensor
  float temperature = dht.readTemperature();

  // Check if temperature is within threshold
  if (temperature < min_temperature_threshold || temperature > max_temperature_threshold) {
    // Temperature out of range, trigger alarm
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Update LCD with current temperature
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print(" C ");

  // Check if the temperature up button is pressed
  if (digitalRead(TEMP_UP_PIN) == LOW) {
    delay(100); // debounce
    if (digitalRead(TEMP_UP_PIN) == LOW) {
      max_temperature_threshold++;
      updateThresholdDisplay();
    }
  }

  // Check if the temperature down button is pressed
  if (digitalRead(TEMP_DOWN_PIN) == LOW) {
    delay(100); // debounce
    if (digitalRead(TEMP_DOWN_PIN) == LOW) {
      min_temperature_threshold--;
      updateThresholdDisplay();
    }
  }

  // Delay for stability
  delay(1000);
}

// Function to update LCD with the current temperature thresholds
void updateThresholdDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Min Temp: ");
  lcd.print(min_temperature_threshold);
  lcd.print(" C ");
  lcd.setCursor(0, 1);
  lcd.print("Max Temp: ");
  lcd.print(max_temperature_threshold);
  lcd.print(" C ");
}
