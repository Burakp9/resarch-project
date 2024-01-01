#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>
#include <RTClib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc;

const int AD8232_PIN = A0;
const int SD_CS_PIN = 5; // Assuming you connect SD card CS pin to GPIO 5

File dataFile;

void setup() {
  Serial.begin(115200);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD Card initialization failed!");
    return;
  }

  display.begin(SSD1306_I2C_ADDRESS, 4, 5);  // Address 0x3D for 128x32
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // Read analog data from AD8232
  int heartRate = analogRead(AD8232_PIN);

  // Get current time from RTC
  DateTime now = rtc.now();

  // Write data to micro SD card
  writeDataToSD(now, heartRate);

  // Display heart rate on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Heart Rate: ");
  display.print(heartRate);
  display.display();

  delay(1000);  // Adjust the delay based on your monitoring requirements
}

void writeDataToSD(DateTime timestamp, int heartRate) {
  String fileName = "/ecg_data.txt";
  dataFile = SD.open(fileName, FILE_WRITE);

  if (dataFile) {
    dataFile.print(timestamp.timestamp());
    dataFile.print(",");
    dataFile.println(heartRate);
    dataFile.close();
  } else {
    Serial.println("Error opening file");
  }
}
