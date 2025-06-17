#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Konfigurasi LCD (Alamat I2C: 0x27 atau 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Pin Sensor Warna TCS3200
#define S0 8   // Skala frekuensi
#define S1 9   // Skala frekuensi
#define S2 12  // Jenis fotodioda (Merah/Hijau/Biru)
#define S3 11  // Jenis fotodioda (Merah/Hijau/Biru)
#define OUT 10 // Output pulsa

// Pin Servo
#define PIN_SERVO 6  

// Kalibrasi Warna (Sesuaikan nilai ini!)
struct Warna {
  int merahR = 200, merahG = 50, merahB = 50;     // Kalibrasi merah
  int hijauR = 50, hijauG = 200, hijauB = 60;     // Kalibrasi hijau
  int coklatR = 150, coklatG = 100, coklatB = 40; // Kalibrasi coklat
  int hitamR = 15, hitamG = 10, hitamB = 10;      // Kalibrasi hitam
} targetWarna;

const int TOLERANSI_WARNA = 30;    // Toleransi umum untuk warna
const int TOLERANSI_HITAM = 15;    // Toleransi ketat untuk hitam

Servo servoWarna;                  // Objek servo
const int POSISI_NETRAL = 90;      // Posisi netral servo
const int POSISI_MERAH = 180;      // Posisi servo untuk merah
const int POSISI_HITAM = 0;        // Posisi servo untuk hitam

// Variabel untuk kontrol servo merah
bool sedangMendeteksiMerah = false;
bool menungguReset = false;
unsigned long waktuDeteksiMerah = 0;
const unsigned long DELAY_RESET = 2000; // Delay 2 detik

void setup() {
  Serial.begin(9600);
  
  // Inisialisasi LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Deteksi Warna");
  
  // Konfigurasi Sensor TCS3200
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  digitalWrite(S0, HIGH);  // Set skala frekuensi 20%
  digitalWrite(S1, LOW);
  
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Attach Servo
  servoWarna.attach(PIN_SERVO);
  servoWarna.write(POSISI_NETRAL);

  delay(1000);
  lcd.clear();
}

void loop() {
  int merah, hijau, biru;
  bacaRGB(merah, hijau, biru);
  tampilkanWarna(merah, hijau, biru);
  kendalikanServo(merah, hijau, biru);
  delay(200);
}

void bacaRGB(int &merah, int &hijau, int &biru) {
  // Baca komponen Merah
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  merah = pulseIn(OUT, LOW);
  merah = map(merah, 0, 255, 255, 0);
  
  // Baca komponen Hijau
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  hijau = pulseIn(OUT, LOW);
  hijau = map(hijau, 0, 255, 255, 0);
  
  // Baca komponen Biru
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  biru = pulseIn(OUT, LOW);
  biru = map(biru, 0, 255, 255, 0);

  merah = constrain(merah, 0, 255);
  hijau = constrain(hijau, 0, 255);
  biru = constrain(biru, 0, 255);
}

void tampilkanWarna(int merah, int hijau, int biru) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R:"); lcd.print(merah);
  lcd.print(" H:"); lcd.print(hijau);
  
  lcd.setCursor(0, 1);
  lcd.print("B:"); lcd.print(biru);

  if (adalahHitam(merah, hijau, biru)) {
    lcd.print(" HITAM");
  }
  else if (adalahWarna(merah, hijau, biru, targetWarna.merahR, targetWarna.merahG, targetWarna.merahB)) {
    lcd.print(" MERAH");
  } 
  else if (adalahWarna(merah, hijau, biru, targetWarna.hijauR, targetWarna.hijauG, targetWarna.hijauB)) {
    lcd.print(" HIJAU");
  }
  else if (adalahWarna(merah, hijau, biru, targetWarna.coklatR, targetWarna.coklatG, targetWarna.coklatB)) {
    lcd.print(" COKLAT");
  }
  else {
    lcd.print(" TIDAK DIKENAL");
  }
}

bool adalahWarna(int merah, int hijau, int biru, int targetR, int targetG, int targetB) {
  return (abs(merah - targetR) < TOLERANSI_WARNA && 
         abs(hijau - targetG) < TOLERANSI_WARNA && 
         abs(biru - targetB) < TOLERANSI_WARNA);
}

bool adalahHitam(int merah, int hijau, int biru) {
  return (merah < targetWarna.hitamR + TOLERANSI_HITAM) &&
         (hijau < targetWarna.hitamG + TOLERANSI_HITAM) &&
         (biru < targetWarna.hitamB + TOLERANSI_HITAM);
}

void kendalikanServo(int merah, int hijau, int biru) {
  bool merahTerbaca = adalahWarna(merah, hijau, biru, targetWarna.merahR, targetWarna.merahG, targetWarna.merahB);
  
  if (merahTerbaca) {
    if (!sedangMendeteksiMerah) {
      // Jika baru mendeteksi merah
      servoWarna.write(POSISI_MERAH);
      lcd.setCursor(13, 1);
      lcd.print("MRH->");
      sedangMendeteksiMerah = true;
      menungguReset = false;
    }
  } 
  else {
    if (sedangMendeteksiMerah && !menungguReset) {
      // Jika merah sudah tidak terdeteksi tapi belum mulai delay
      waktuDeteksiMerah = millis();
      menungguReset = true;
    }
    
    if (menungguReset && (millis() - waktuDeteksiMerah >= DELAY_RESET)) {
      // Jika sudah menunggu 2 detik setelah merah hilang
      servoWarna.write(POSISI_NETRAL);
      sedangMendeteksiMerah = false;
      menungguReset = false;
      lcd.setCursor(13, 1);
      lcd.print("NETRAL");
    }
  }
  
  // Kontrol untuk warna hitam (prioritas lebih tinggi)
  if (adalahHitam(merah, hijau, biru)) {
    servoWarna.write(POSISI_HITAM);
    lcd.setCursor(13, 1);
    lcd.print("<-HTM");
    sedangMendeteksiMerah = false;
    menungguReset = false;
  }
}
