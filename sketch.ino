#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// Simulated optical sensor
const int sensorPin = 34;

// RGB LED
const int redPin = 27;
const int greenPin = 26;
const int bluePin = 25;

// Buzzer
const int buzzerPin = 14;

void setup() {

  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  // Start OLED
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS)) {

    Serial.println("OLED failed!");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("H2S");

  display.setTextSize(1);
  display.setCursor(10, 35);
  display.println("Safety Badge");

  display.display();

  delay(2000);
}

void setRGB(bool red, bool green, bool blue) {

  digitalWrite(redPin, red);
  digitalWrite(greenPin, green);
  digitalWrite(bluePin, blue);
}

void loop() {

  // Read simulated optical signal
  int sensorValue = analogRead(sensorPin);

  Serial.print("Sensor value: ");
  Serial.println(sensorValue);

  String status;

  // SAFE
  if (sensorValue < 1300) {

    status = "SAFE";

    setRGB(false, true, false);

    noTone(buzzerPin);
  }

  // WARNING
  else if (sensorValue < 2800) {

    status = "WARNING";

    setRGB(true, true, false);

    noTone(buzzerPin);
  }

  // DANGER
  else {

    status = "DANGER";

    setRGB(true, false, false);

    tone(buzzerPin, 2000);
  }

  // OLED
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10, 5);
  display.println("H2S");

  display.setTextSize(1);

  display.setCursor(10, 30);
  display.print("Signal: ");
  display.println(sensorValue);

  display.setCursor(10, 45);
  display.print("Status: ");
  display.println(status);

  display.display();

  delay(300);
}
