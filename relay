// Pin Definitions
const int trigPin = D1;  // Pin Trig (Trigger) for HC-SR04
const int echoPin = D2;  // Pin Echo for HC-SR04
const int relayPin = D3; // Pin for the relay control (Normally Closed)

// Variables
long duration; // Duration of the ultrasonic pulse
int distance;  // Calculated distance in centimeters
unsigned long lastTriggerTime = 0; // Timestamp of the last trigger
bool relayState = false; // Relay state (false = Normally Open, true = Normally Closed)

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Configure pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);

  // Ensure the relay is initially Normally Open
  digitalWrite(relayPin, HIGH); // HIGH = Normally Open (relay off)
}

void loop() {
  // Step 1: Measure distance using the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Measure the pulse duration
  distance = duration * 0.034 / 2;   // Convert to distance in centimeters

  // Debug: Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  // Step 2: Check if an object is detected (distance < 10 cm)
  if (distance < 10 && !relayState) {
    // Activate the relay (Normally Closed)
    digitalWrite(relayPin, LOW); // LOW = Normally Closed (relay on)
    relayState = true;           // Update relay state
    lastTriggerTime = millis();  // Record the trigger time
    Serial.println("Relay Activated (Normally Closed)");
  }

  // Step 3: Wait for 5 seconds, then deactivate the relay
  if (relayState && (millis() - lastTriggerTime >= 5000)) {
    // Deactivate the relay (Normally Open)
    digitalWrite(relayPin, HIGH); // HIGH = Normally Open (relay off)
    relayState = false;           // Update relay state
    Serial.println("Relay Deactivated (Normally Open)");
  }

  // Add a small delay to avoid rapid readings
  delay(100);
}
