/*
   Send out bytes in proper format.
*/

#include "common.h"
#include <SPI.h>

void setup() {
  // initialize digital pin 13 as an output.

  pinMode(SPI_SS_PIN, OUTPUT);
  SPI.begin();
}

void loop() {
  delay(DELAY);              // wait for a second
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  digitalWrite(SPI_SS_PIN, LOW);

  SPI.transfer(0xAA);
  SPI.transfer(0x55);
  SPI.transfer(0xAA);
  SPI.transfer(0x55);

  // take the SS pin high to de-select the chip:
  digitalWrite(SPI_SS_PIN, HIGH);
  SPI.endTransaction();

}