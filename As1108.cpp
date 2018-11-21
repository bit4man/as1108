/*
 * as1108 7segment driver
 * Legacy chipset to drive up to 4 7-segment displays to display nummeric data.
 */

#include "Arduino.h"
#include "As1108.h"
#include <SPI.h>
#define SEGMENTS 4

As1108::As1108(int slavePin) {
  _slavePin=slavePin;
  pinMode(_slavePin, OUTPUT);
  digitalWrite(_slavePin, HIGH);
  feature =BinarySet<feature_t>(0b00001000);
  decode  =BinarySet<decode_t>(0x0f);
  shutdown=BinarySet<shutdown_t>(0x01);
	intensity=0x02;
	scanlimit=0x03;
}

void As1108::init() {
  // initialize SPI:
  SPI.begin();
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
	sendShutdown(); 
	sendFeature();
	sendIntensity();
	sendDecodeMode(); 
	sendIntensity();
	sendScanLimit();
	sendData(DISPTEST,0x00);
  SPI.endTransaction();
  delay(20);
  displayNumber(0);  
};

void As1108::sendShutdown() {
  sendData(SHUTDOWN, shutdown.getFlags());
};

void As1108::sendFeature() {
  sendData(FEATURE, feature.getFlags());
};

void As1108::setScanLimit(byte numSegments ) {
  _scanlimit=numSegments=1; // 0=1 segment, 1=2 segments etc 
}

void As1108::sendDecodeMode() {
  sendData(DECODE_MODE, decode.getFlags());
}

void As1108::sendIntensity() {
  sendData(INTENSITY, intensity);
}

void As1108::sendScanLimit() {
  sendData(SCAN_LIMIT, _scanlimit);
}

void As1108::showDigits(dispseg_t d) {
  unsigned int p = SEGMENTS-1;
  while ((p>0) && (d[p]==0)) { // Do not blank the first digit
    d[p--]=0x0f; // Blank if Leading Zero 
  }
  for (p=0; p<SEGMENTS; p++) {
    sendData(p+DIG0, d[p]); // Display digit
  }
}

void As1108::sendData(int address, int value) {
  // Select the chip/enable it
//  char str[80];
//  sprintf(str,"Address: %02x, value: %02x\n", address, value);
//  Serial.print(str);
  digitalWrite(_slavePin, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(_slavePin, HIGH);
}

boolean As1108::displayNumber(double num) {
  _displayValue=num;
  int digit=0x00, n=abs(num);
  const boolean isNeg= num < 0 ? true : false;
  int dec=(abs(num)-(int)n)*10;
  byte decPos;
  if (dec>0) {
     decPos = 1;
     n=n*10+dec; // Add decimal portion to integer
  } else decPos = 0xFF;
     
  byte maxdigit=0;
  while (digit < 4) {
    int r = n % 10;
    if (r>0) maxdigit=digit;
    if (digit == decPos) r|=0x80; // Add decimal point
    _digitsegment[digit++] = r;
    n = trunc(n / 10);
  }
  if (isNeg) {
    _digitsegment[maxdigit+1] = 0x0A; // Negative sign
    maxdigit++;
  }

  if (n > 0) return false;
  showDigits(_digitsegment);
  return true;
}
