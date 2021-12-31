#include "FS.h"
#include "SD_MMC.h"

#define BUFFER_SIZE   8192
#define GPS_BAUD_RATE 460800
#define GPS_RX        3
#define GPS_TX        1

uint8_t bufferSend[BUFFER_SIZE];
uint16_t i2 = 0;

HardwareSerial *hwSerial = nullptr;

char filename[40];
File file;

void setup() {
  Serial.begin(115200);

  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);

  if (hwSerial == nullptr)
  {
    hwSerial = new HardwareSerial(0);
  }

  hwSerial->begin(115200);

  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }
  else {
    Serial.println("SD card detected");
  }

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  sprintf(filename, "/rover_%1d.ubx", millis());
  Serial.println(filename);
  file = SD_MMC.open(filename, FILE_APPEND);
  if (!file) {
    Serial.println("File open error");
  }
}

void loop() {
  if (hwSerial->available())
  {
    while (hwSerial->available())
    {
      bufferSend[i2] = (char)hwSerial->read();
      if (i2 < BUFFER_SIZE - 1)
      {
        i2++;
      }
    }

    file.write(bufferSend, i2);
    file.print("Data Received");
    Serial.println("Data Received");
    i2 = 0;
  }

  delay(125);
}
