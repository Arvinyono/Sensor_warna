const int trigPin = D1;   // GPIO5
const int echoPin = D2;   // GPIO4
const int relayPin = D5;  // GPIO14 (Ganti ke pin yang lebih aman)
const int ledPin = D4;    // GPIO2 (LED onboard)

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(relayPin, HIGH); // Matikan relay awal
  digitalWrite(ledPin, HIGH);   // Matikan LED onboard (LOW untuk nyala)
}

void loop() {
  // Baca sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Kontrol relay dan LED
  if (distance < 10 && !digitalRead(relayPin)) {
    digitalWrite(relayPin, LOW);  // Relay ON
    digitalWrite(ledPin, LOW);    // LED ON (jika pakai onboard)
    Serial.println("Objek dekat - Relay ON");
    delay(5000);                  // Tunda 5 detik
    digitalWrite(relayPin, HIGH); // Relay OFF
    digitalWrite(ledPin, HIGH);   // LED OFF
    Serial.println("Relay OFF");
  }
  delay(100);
}
