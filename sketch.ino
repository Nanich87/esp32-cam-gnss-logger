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

  if (hwSerial == nullptr)
  {
    hwSerial = new HardwareSerial(1);
  }

  hwSerial->begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX, GPS_TX);

  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }

  sprintf(filename, "rover_%1d.ubx", millis());
  file = SD_MMC.open(filename, FILE_APPEND);
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
    i2 = 0;
  }
}
