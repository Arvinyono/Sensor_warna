const int relayPin = D3;  // Ganti dengan pin GPIO yang sesuai (contoh: D1 = GPIO5)
const int ledPin = D2;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Matikan relay awal (Normally Closed)
  digitalWrite(ledPin, HIGH);
  Serial.println("Relay Control Started");
}

void loop() {
  // Aktifkan relay (Normally Open = ON)
  digitalWrite(relayPin, LOW);  // LOW untuk mengaktifkan relay
  digitalWrite(ledPin, LOW);
  Serial.println("Relay ON (Normally Open)");
  delay(3000); // Tahan 3 detik

  // Matikan relay (Normally Closed = OFF)
  digitalWrite(relayPin, HIGH); // HIGH untuk mematikan relay
  digitalWrite(ledPin, HIGH);
  Serial.println("Relay OFF (Normally Closed)");
  delay(3000); // Tahan 3 detik
}
