#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------------------------
// Pin Configuration
// -------------------------

#define SENSOR_PIN 34
#define LED_PIN 2
#define BUZZER_PIN 27

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDRESS 0x3C

// -------------------------
// OLED Configuration
// -------------------------

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// -------------------------
// Setup
// -------------------------

void setup()
{
    Serial.begin(115200);

    // Pins
    pinMode(SENSOR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    // I2C
    Wire.begin(OLED_SDA, OLED_SCL);

    // OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
    {
        Serial.println("OLED initialization failed!");
    }
    else
    {
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);

        display.setTextSize(2);
        display.setCursor(10, 5);
        display.println("H2S");

        display.setTextSize(1);
        display.setCursor(10, 30);
        display.println("Safety Monitor");

        display.display();

        delay(1500);
    }

    Serial.println("H2S Safety Monitor Started");
}

// -------------------------
// Main Loop
// -------------------------

void loop()
{
    // Read sensor / potentiometer value
    int sensorValue = analogRead(SENSOR_PIN);

    // Convert to percentage
    int level = map(sensorValue, 0, 4095, 0, 100);

    // Keep value within range
    level = constrain(level, 0, 100);

    // Show value in Serial Monitor
    Serial.print("Sensor value: ");
    Serial.println(sensorValue);

    Serial.print("H2S level: ");
    Serial.print(level);
    Serial.println("%");

    // -------------------------
    // Display
    // -------------------------

    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("H2S");

    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print("Sensor: ");
    display.println(sensorValue);

    display.setCursor(0, 40);
    display.print("Level: ");
    display.print(level);
    display.println("%");

    // -------------------------
    // Alert Logic
    // -------------------------

    if (level >= 70)
    {
        // HIGH H2S indication
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);

        display.setCursor(0, 55);
        display.println("DANGER!");
    }
    else if (level >= 40)
    {
        // WARNING
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, LOW);

        display.setCursor(0, 55);
        display.println("WARNING");
    }
    else
    {
        // SAFE
        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);

        display.setCursor(0, 55);
        display.println("SAFE");
    }

    display.display();

    delay(1000);
}