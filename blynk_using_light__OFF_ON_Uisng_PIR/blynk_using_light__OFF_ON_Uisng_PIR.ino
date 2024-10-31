// Define the pin numbers for ESP8266
const int pirSensorPin = 5; // GPIO 5 on ESP8266 (D1 on NodeMCU)
const int ledPin = 4;         // GPIO 4 on ESP8266 (D2 on NodeMCU)

// Fill-in information from Blynk Device Info here
#define BLYNK_TEMPLATE_ID           "TMPL6_4BKx73A"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "C5WqL44YR4AXwCcKeuuqNFCw-jKxtOBL"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials
char ssid[] = "CPH1609";        // Your WiFi SSID
char pass[] = "1234567R";       // Your WiFi Password

BlynkTimer timer;

void pirSensorControl()
{
  // Read the state of the touch sensor
  int sensorValue = digitalRead(pirSensorPin);
  Serial.println(sensorValue);

  // If the sensor is touched, turn on the LED and send the status to Blynk
  if (sensorValue == HIGH) {
    digitalWrite(ledPin, HIGH);         // Turn on LED
    Blynk.virtualWrite(V1, 255);        // Send ON status to Blynk
  }
  // If the sensor is not touched, turn off the LED and update Blynk
  else {
    digitalWrite(ledPin, LOW);          // Turn off LED
    Blynk.virtualWrite(V1, 0);          // Send OFF status to Blynk
  }
}

// This function sends Arduino's uptime every second to Virtual Pin 2
void myTimerEvent()
{
  // Send uptime to Blynk app (V2)
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Set pin modes
  pinMode(pirSensorPin, INPUT);  // Set touch sensor pin as input
  pinMode(ledPin, OUTPUT);         // Set LED pin as output

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Set up a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(100L, pirSensorControl);  // Check touch sensor every 100 ms
}

void loop()
{
  Blynk.run();  // Run Blynk
  timer.run();  // Run the timer
}