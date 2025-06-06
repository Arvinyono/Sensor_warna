#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD I2C (address biasanya 0x27 atau 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Pin Sensor TCS3200
#define S0 8
#define S1 9
#define S2 12
#define S3 11
#define OUT 10

// Variabel warna
int red, green, blue;

// Kalibrasi manual (sesuaikan dengan pengukuran Anda)
int hijauR = 50, hijauG = 200, hijauB = 60;    // Contoh nilai hijau
int coklatR = 150, coklatG = 100, coklatB = 40; // Contoh nilai coklat
int toleransi = 30; // Toleransi deteksi warna

void setup() {
  Serial.begin(9600);
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Deteksi Warna");
  
  // Set frekuensi sensor TCS3200 (20%)
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  delay(1000);
  lcd.clear();
}

void loop() {
  bacaWarna();
  tampilkanWarna();
  delay(1000);
}

void bacaWarna() {
  // Baca Red
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  red = pulseIn(OUT, LOW);
  red = map(red, 0, 255, 255, 0); // Inversi nilai
  
  // Baca Green
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  green = pulseIn(OUT, LOW);
  green = map(green, 0, 255, 255, 0);
  
  // Baca Blue
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blue = pulseIn(OUT, LOW);
  blue = map(blue, 0, 255, 255, 0);

  // Batasi nilai 0-255
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

  // Deteksi warna
  if (isColor(hijauR, hijauG, hijauB)) {
    lcd.print(" HIJAU");
  } else if (isColor(coklatR, coklatG, coklatB)) {
    lcd.print(" COKLAT");
  } else {
    lcd.print(" TIDAK DIKENALI");
  }
}

// Fungsi deteksi warna dengan toleransi
bool isColor(int targetR, int targetG, int targetB) {
  return (abs(red - targetR) < toleransi) && 
         (abs(green - targetG) < toleransi) && 
         (abs(blue - targetB) < toleransi);
}
