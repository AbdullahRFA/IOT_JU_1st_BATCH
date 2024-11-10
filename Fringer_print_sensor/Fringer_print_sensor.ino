#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Set up SoftwareSerial (change pins according to your setup)
SoftwareSerial mySerial(0, 1); // RX, TX pins for the fingerprint sensor

// Initialize the fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Array of user IDs mapped to usernames
const char* users[] = {
  "No Match",         // Placeholder for ID 0 or "no match"
  "Alice",            // ID 1
  "Bob",              // ID 2
  "Charlie",          // ID 3
  "David",            // ID 4
  "Eve"               // ID 5
  // Add more usernames as needed
};

void setup() {
  Serial.begin(9600);        // Start serial monitor for debugging
  mySerial.begin(57600);     // Start the sensor at its default baud rate
  finger.begin(57600);       // Pass baud rate as an argument to begin()

  // Initialize the fingerprint sensor
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor found.");
  } else {
    Serial.println("Fingerprint sensor not found. Check connections.");
    while (1); // Stop program if sensor is not found
  }

  // Display the number of fingerprints enrolled
  finger.getTemplateCount(); 
  if (finger.templateCount == 0) {
    Serial.println("No fingerprints enrolled. Please enroll fingerprints first.");
  } else {
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates.");
  }
}

void loop() {
  Serial.println("Place your finger on the sensor...");

  // Capture and identify the fingerprint
  int userID = getFingerprintID();

  // Output the result
  if (userID >= 0 && userID < sizeof(users) / sizeof(users[0])) {
    Serial.print("User matched: ");
    Serial.println(users[userID]);
  } else {
    Serial.println("No match found.");
  }

  delay(2000); // Wait 2 seconds for a new fingerprint scan
}

// Function to capture and return the fingerprint ID
int getFingerprintID() {
  // Attempt to capture the fingerprint image
  int result = finger.getImage();
  if (result != FINGERPRINT_OK) {
    Serial.println("Failed to capture fingerprint.");
    return -1;
  }

  // Convert the image into characteristics
  result = finger.image2Tz();
  if (result != FINGERPRINT_OK) {
    Serial.println("Failed to convert image.");
    return -1;
  }

  // Search for a matching fingerprint ID
  result = finger.fingerFastSearch();
  if (result == FINGERPRINT_OK) {
    Serial.print("Match found! User ID: ");
    Serial.println(finger.fingerID);
    return finger.fingerID; // Return the matched user ID
  } else {
    Serial.println("No match found.");
    return -1; // No match
  }
}