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

struct Warna {
  int merahR = 200, merahG = 50, merahB = 50;
  int hijauR = 50, hijauG = 200, hijauB = 60;
  int coklatR = 150, coklatG = 100, coklatB = 40;
} targetWarna;

const int toleransi = 30;
Servo myservo;
const int posAwal = 90;
const int posMerah = 180;
bool statusMerah = false;

// Variabel untuk delay servo
unsigned long waktuDeteksiMerah = 0;
const unsigned long delayServo = 2000;  // 2 detik

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
  int red, green, blue;
  bacaWarna(red, green, blue);
  tampilkanWarna(red, green, blue);
  kontrolServo(red, green, blue);
  delay(200);
}

void bacaWarna(int &red, int &green, int &blue) {
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

void tampilkanWarna(int red, int green, int blue) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R:"); lcd.print(red);
  lcd.print(" G:"); lcd.print(green);
  
  lcd.setCursor(0, 1);
  lcd.print("B:"); lcd.print(blue);

  if (deteksiWarna(red, green, blue, targetWarna.merahR, targetWarna.merahG, targetWarna.merahB)) {
    lcd.print(" MERAH");
  } 
  else if (deteksiWarna(red, green, blue, targetWarna.hijauR, targetWarna.hijauG, targetWarna.hijauB)) {
    lcd.print(" HIJAU");
  }
  else if (deteksiWarna(red, green, blue, targetWarna.coklatR, targetWarna.coklatG, targetWarna.coklatB)) {
    lcd.print(" COKLAT");
  }
  else {
    lcd.print(" TIDAK DIKENALI");
  }
}

bool deteksiWarna(int red, int green, int blue, int targetR, int targetG, int targetB) {
  return (abs(red - targetR) < toleransi && 
         abs(green - targetG) < toleransi && 
         abs(blue - targetB) < toleransi);
}

void kontrolServo(int red, int green, int blue) {
  bool merahTerbaca = deteksiWarna(red, green, blue, targetWarna.merahR, targetWarna.merahG, targetWarna.merahB);
  
  if (merahTerbaca && !statusMerah) {
    myservo.write(posMerah);
    lcd.setCursor(13, 1);
    lcd.print("->");
    statusMerah = true;
    waktuDeteksiMerah = millis();  // Catat waktu deteksi
  } 
  else if (!merahTerbaca && statusMerah) {
    if (millis() - waktuDeteksiMerah >= delayServo) {  // Tunggu 2 detik
      myservo.write(posAwal);
      statusMerah = false;
    }
  }
}
