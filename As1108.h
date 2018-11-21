/*
 * as1108 7segment driver
 * Legacy chipset to drive up to 4 7-segment displays to display nummeric data.
 * Capabilities:
 * - Sleep
 *   Can go to low power mode
 * - Display up to 4 digit integers
 * - Display negagive numbers with 3 digits
 * - Display up to 2 decimal points - floating
 *
 * To-do:
 *  Slave a second driver/display for larger numbers
 */

#ifndef As1108_h
#define As1108_h

#include "BinarySet.h"
#include "Arduino.h"

typedef enum shutdownCodes {
			 SHUTDOWN_DEFAULT=0x00     // Shutdown. Reset Feature Register to Default Settings
	   , SHUTDOWN_NORMAL=0x80      // Shutdown. Do not change feature registry 
		 , SHUTDOWN_RESTORE_RESET=0x01  // Normal Operations - reset Feature Register
		 , SHUTDOWN_RESTORE_NORMAL=0x81 // Normal Operations - do not change feature registry
} shutdown_t;

typedef enum features {
				clk_en=0x01,
				reg_res=0x02,
				deccode_sel=0x04,
				spi_en=0x08,
				blink_en=0x10,
				blink_freq_sel=0x20,
				sync=0x40,
				blink_start=0x80
} feature_t;

typedef enum decodeCodes {
	NO_DECODE=0x00
 ,SEG0_DECODE=0x01
 ,SEG1_DECODE=0x02
 ,SEG2_DECODE=0x04
 ,SEG3_DECODE=0x08
 ,ALL_DECODE=0xFF 
} decode_t;


class As1108 {
  typedef byte dispseg_t[4];
  dispseg_t _digitsegment = {0, 0, 0, 0};
  double _displayValue;

  const byte 
			NOOP=0x00,
		  DIG0=0x01,
			DIG1=0x02,
			DIG2=0x03,
			DIG3=0x04,
			DECODE_MODE=0x09,
			INTENSITY=0x0A,
			SCAN_LIMIT=0x0B,
			SHUTDOWN=0x0C,
			FEATURE=0x0E,
			DISPTEST=0x0F;
  int _slavePin;
  void sendData(int address, int value);
  byte _scanlimit = 0x03;

	void sendFeature(); 
	void sendShutdown(); 
  void sendDecodeMode();
  void sendIntensity();
	void sendScanLimit();
	void showDigits(dispseg_t d);

public:
  As1108(int slavePin) ;
	void init();
  void dispTest();
	boolean displayNumber(double num);
  BinarySet<feature_t> feature;
  BinarySet<decode_t> decode;
  BinarySet<shutdown_t> shutdown;
  byte intensity=0, scanlimit=0;
  void setScanLimit(byte numSegments);
};


#endif

