const int ledPin = D4; // GPIO2 (LED onboard Wemos)

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(relayPin, LOW);  // Relay ON
  digitalWrite(ledPin, LOW);    // LED ON (active LOW untuk LED onboard)
  delay(3000);
  
  digitalWrite(relayPin, HIGH); // Relay OFF
  digitalWrite(ledPin, HIGH);   // LED OFF
  delay(3000);
}
