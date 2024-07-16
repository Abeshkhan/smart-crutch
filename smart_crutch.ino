#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <MPU6050.h>

// Define pin connections
const int ledPin = 9; // PWM pin for LED
const int buzzerPin = 8;
const int lightLedPin = 10;
const int ldrPin = A0;

// Create sensor objects
Adafruit_BMP280 bmp; // I2C
MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  
  // Initialize BMP280 sensor
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  // Initialize MPU6050 sensor
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println(F("MPU6050 connection failed!"));
    while (1);
  }

  // Set pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightLedPin, OUTPUT);
}

void loop() {
  // Read pressure from BMP280 sensor
  float pressure = bmp.readPressure();
  Serial.print(F("Pressure = "));
  Serial.print(pressure);
  Serial.println(" Pa");

  // Map pressure to LED brightness (assuming 90000 Pa to 110000 Pa range)
  int ledBrightness = map(pressure, 90000, 110000, 0, 255);
  analogWrite(ledPin, ledBrightness);

  // Read accelerometer data from MPU6050
  Vector rawAccel = mpu.readRawAccel();
  int16_t ax = rawAccel.XAxis;
  int16_t ay = rawAccel.YAxis;
  int16_t az = rawAccel.ZAxis;

  // Check for fall condition
  if (abs(ax) > 15000 || abs(ay) > 15000 || abs(az) < 5000) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // Read light level from LDR
  int ldrValue = analogRead(ldrPin);
  Serial.print(F("LDR Value = "));
  Serial.println(ldrValue);

  // Turn on light LED in low-light conditions
  if (ldrValue < 300) {
    digitalWrite(lightLedPin, HIGH);
  } else {
    digitalWrite(lightLedPin, LOW);
  }

  delay(500);
}
