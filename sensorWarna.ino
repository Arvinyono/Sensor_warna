#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
#define S0 8
#define S1 9
#define S2 12
#define S3 11
#define OUT 10
#define SERVO_PIN 6

// Variabel warna
int red, green, blue;

// Kalibrasi (sesuaikan!)
int hijauR = 50, hijauG = 200, hijauB = 60;
int coklatR = 150, coklatG = 100, coklatB = 40;
int merahR = 200, merahG = 50, merahB = 50; // Ganti dengan nilai kalibrasi Anda
int toleransi = 30;

Servo myservo;
int posAwal = 90;
int posAktif = 180;
bool merahTerdeteksi = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Deteksi Warna");
  
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  myservo.attach(SERVO_PIN);
  myservo.write(posAwal);
  delay(1000);
  lcd.clear();
}

void loop() {
  bacaWarna();
  tampilkanWarna();
  kontrolServo();
  delay(200); // Respons lebih cepat
}

void bacaWarna() {
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  red = pulseIn(OUT, LOW);
  red = map(red, 0, 255, 255, 0);
  
  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  green = pulseIn(OUT, LOW);
  green = map(green, 0, 255, 255, 0);
  
  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  blue = pulseIn(OUT, LOW);
  blue = map(blue, 0, 255, 255, 0);

  red = constrain(red, 0, 255);
  green = constrain(green, 0, 255);
  blue = constrain(blue, 0, 255);
}

void tampilkanWarna() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R:"); lcd.print(red);
  lcd.print(" G:"); lcd.print(green);
  
  lcd.setCursor(0, 1);
  lcd.print("B:"); lcd.print(blue);

  if (isColor(merahR, merahG, merahB)) {
    lcd.print(" MERAH");
  } 
  else if (isColor(hijauR, hijauG, hijauB)) {
    lcd.print(" HIJAU");
  }
  else if (isColor(coklatR, coklatG, coklatB)) {
    lcd.print(" COKLAT");
  }
  else {
    lcd.print(" TIDAK DIKENALI");
  }
}

bool isColor(int targetR, int targetG, int targetB) {
  return (abs(red - targetR) < toleransi) && 
         (abs(green - targetG) < toleransi) && 
         (abs(blue - targetB) < toleransi);
}

void kontrolServo() {
  if (isColor(merahR, merahG, merahB)) {
    if (!merahTerdeteksi) {
      myservo.write(posAktif);
      lcd.setCursor(13, 1);
      lcd.print("->");
      merahTerdeteksi = true; // Update status setelah servo bergerak
    }
  } else {
    if (merahTerdeteksi) {
      myservo.write(posAwal);
      merahTerdeteksi = false;
    }
  }
}
