#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Pin definitions
#define GREEN_LED 7
#define RED_LED 8

// Fingerprint sensor setup
SoftwareSerial fingerSerial(2, 3); // RX, TX pins for fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

void setup() {
  Serial.begin(9600);
  
  // Initialize LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  // Initialize fingerprint sensor
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor found!");
  } else {
    Serial.println("Fingerprint sensor not detected.");
    while (1); // Halt if sensor is not found
  }
}

void loop() {
  Serial.println("Place your finger on the sensor...");
  
  int id = getFingerprintID(); // Get fingerprint ID
  
  if (id >= 0) {
    Serial.println("Access Granted");
    digitalWrite(GREEN_LED, HIGH);  // Turn on green LED
    delay(3000);                    // Keep green LED on for 3 seconds
    digitalWrite(GREEN_LED, LOW);   // Turn off green LED
  } else {
    Serial.println("Access Denied");
    digitalWrite(RED_LED, HIGH);    // Turn on red LED
    delay(3000);                    // Keep red LED on for 3 seconds
    digitalWrite(RED_LED, LOW);     // Turn off red LED
  }
  delay(2000); // Delay before next attempt
}

int getFingerprintID() {
  int result = finger.getImage();
  if (result != FINGERPRINT_OK) return -1;

  result = finger.image2Tz();
  if (result != FINGERPRINT_OK) return -1;

  result = finger.fingerFastSearch();
  if (result != FINGERPRINT_OK) return -1;

  return finger.fingerID;
}
