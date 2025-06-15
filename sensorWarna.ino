#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Inisialisasi LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Pin Sensor TCS3200
#define S0 8
#define S1 9
#define S2 12
#define S3 11
#define OUT 10

// Pin Servo
#define SERVO_PIN 6

// Variabel warna
int red, green, blue;

// Kalibrasi warna (sesuaikan dengan pengukuran)
int hijauR = 50, hijauG = 200, hijauB = 60;
int coklatR = 150, coklatG = 100, coklatB = 40;
int merahR = 200, merahG = 50, merahB = 50; // Nilai untuk merah
int toleransi = 30;

// Variabel Servo
Servo myservo;
int posAwal = 90; // Posisi awal/netral
int posAktif = 180; // Posisi ketika mendeteksi merah
bool merahTerdeteksi = false;

void setup() {
  Serial.begin(9600);
  
  // Inisialisasi LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Deteksi Warna");
  
  // Konfigurasi sensor TCS3200
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Inisialisasi Servo
  myservo.attach(SERVO_PIN);
  myservo.write(posAwal);

  delay(1000);
  lcd.clear();
}

void loop() {
  bacaWarna();
  tampilkanWarna();
  kontrolServo();
  delay(1000);
}

void bacaWarna() {
  // Baca komponen warna
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  red = pulseIn(OUT, LOW);
  red = map(red, 0, 255, 255, 0);
  
  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  green = pulseIn(OUT, LOW);
  green = map(green, 0, 255, 255, 0);
  
  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  blue = pulseIn(OUT, LOW);
  blue = map(blue, 0, 255, 255, 0);

  // Batasi nilai
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
    merahTerdeteksi = true;
  } 
  else if (isColor(hijauR, hijauG, hijauB)) {
    lcd.print(" HIJAU");
    merahTerdeteksi = false;
  }
  else if (isColor(coklatR, coklatG, coklatB)) {
    lcd.print(" COKLAT");
    merahTerdeteksi = false;
  }
  else {
    lcd.print(" TIDAK DIKENALI");
    merahTerdeteksi = false;
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
      myservo.write(posAktif); // Pindah ke kanan
      lcd.setCursor(13, 1);
      lcd.print("->");
    }
  } else {
    if (merahTerdeteksi) {
      myservo.write(posAwal); // Kembali ke posisi awal
    }
  }
}
