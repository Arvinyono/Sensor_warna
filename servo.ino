#include <Servo.h>

Servo myservo;
int servoPin = D4;  // Pin GPIO D4 (bisa diganti)

// Variabel untuk posisi servo
int posOpen = 90;   // Sudut "buka" (sesuaikan)
int posClose = 0;   // Sudut "tutup" (sesuaikan)

unsigned long previousMillis = 0;
const long interval = 3000;  // Interval 3 detik (3000 ms)

void setup() {
  myservo.attach(servoPin);
  myservo.write(posClose);  // Mulai dalam posisi "tutup"
}

void loop() {
  unsigned long currentMillis = millis();

  // Cek jika sudah 3 detik
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Reset waktu

    // Gerakkan servo ke posisi "buka"
    myservo.write(posOpen);
    delay(500);  // Tunggu 0.5 detik (opsional)

    // Kembali ke posisi "tutup" (jika ingin menutup lagi)
    myservo.write(posClose);
  }
}
