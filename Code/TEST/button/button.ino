#define buttonDenSuoi 31
#define buttonQuatDHT11 33
#define buttonPhunSuong 35
#define buttonDenTrangTrai 37
#define buttonChoHeoAn 39
#define buttonVeSinh 41
#define pinMaySuoi 5
#define pinQuatDHT11 6
#define pinPhunSuong 7
#define pinLedTrangTrai 8
#define pinChoAn 9
#define pinVeSinh A0

bool tempDenSuoi = LOW;
bool tempQuatDHT11 = LOW;
bool tempPhunSuong = LOW;
bool tempDenTrangTrai = LOW;
bool tempChoHeoAn = LOW;
bool tempVeSinh = LOW;

void setup() {
  pinMode(buttonDenSuoi, INPUT_PULLUP);
  pinMode(buttonQuatDHT11, INPUT_PULLUP);
  pinMode(buttonPhunSuong, INPUT_PULLUP);
  pinMode(buttonDenTrangTrai, INPUT_PULLUP);
  pinMode(buttonChoHeoAn, INPUT_PULLUP);
  pinMode(buttonVeSinh, INPUT_PULLUP);
  pinMode(pinMaySuoi, OUTPUT);
  pinMode(pinQuatDHT11, OUTPUT);
  pinMode(pinPhunSuong, OUTPUT);
  pinMode(pinLedTrangTrai, OUTPUT);
  pinMode(pinChoAn, OUTPUT);
  pinMode(pinVeSinh, OUTPUT);
}

void loop() {
  if (digitalRead(buttonDenSuoi) == LOW) 
  {
    delay(10);
    if (digitalRead(buttonDenSuoi) == LOW) {
      tempDenSuoi = !tempDenSuoi;
      digitalWrite(pinMaySuoi, tempDenSuoi);
    }
    while (digitalRead(buttonDenSuoi) == LOW);
  } 

  else if (digitalRead(buttonQuatDHT11) == LOW)
  {
    delay(10);
    if (digitalRead(buttonQuatDHT11) == LOW) {
      tempQuatDHT11 = !tempQuatDHT11;
      digitalWrite(pinQuatDHT11, tempQuatDHT11);
    }
    while (digitalRead(buttonQuatDHT11) == LOW);
  } 

  else if (digitalRead(buttonPhunSuong) == LOW) 
  {
    delay(10);
    if (digitalRead(buttonPhunSuong) == LOW) {
      tempPhunSuong = !tempPhunSuong;
      digitalWrite(pinPhunSuong, tempPhunSuong);
    }
    while (digitalRead(buttonPhunSuong) == LOW);
  } 

  else if (digitalRead(buttonDenTrangTrai) == LOW) 
  {
    delay(10);
    if (digitalRead(buttonDenTrangTrai) == LOW) {
      tempDenTrangTrai = !tempDenTrangTrai;
      digitalWrite(pinLedTrangTrai, tempDenTrangTrai);
    }
    while (digitalRead(buttonDenTrangTrai) == LOW);
  } 

  else if (digitalRead(buttonChoHeoAn) == LOW) 
  {
    delay(10);
    if (digitalRead(buttonChoHeoAn) == LOW) {
      tempChoHeoAn = !tempChoHeoAn;
      digitalWrite(pinChoAn, tempChoHeoAn);
    }
    while (digitalRead(buttonChoHeoAn) == LOW);
  }

  else if (digitalRead(buttonVeSinh) == LOW) 
  {
    delay(10);
    if (digitalRead(buttonVeSinh) == LOW) {
      tempVeSinh = !tempVeSinh;
      digitalWrite(pinVeSinh, tempVeSinh);
    }
    while (digitalRead(buttonVeSinh) == LOW);
  }

}
