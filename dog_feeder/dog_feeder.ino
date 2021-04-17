#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>
#include "SENSOR.h"
#include "TIME.h"

/**
 * GSM Config
 */
#define FONA_RX 11
#define FONA_TX 10
#define FONA_RST 9

char replybuffer[255];
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

/**
 * Ultrasonic Config
 */
// main container sensor
#define ECHO_PIN1 A0
#define TRIG_PIN1 A1

// dog bowl sensor
#define ECHO_PIN2 A2
#define TRIG_PIN2 A3

SENSOR sensorCont;

/**
 * TIME config
 */
TIME timeCont;


void setup() {
  while (!Serial);
  Serial.begin(115200);
  
  GSM_setup();
  sensorCont.SENSOR_setup(TRIG_PIN1, ECHO_PIN1, TRIG_PIN2, ECHO_PIN2);
  timeCont.TIME_setup();
}

  
char fonaNotificationBuffer[64];
char smsBuffer[250];


void loop() {
   smsListener();
   sensorCont.updateSensorValueMain();
   sensorCont.updateSensorValueBowl1();
   timeCont.updateTime();




   
}


/**
 * GSM Setup
 */

void GSM_setup() {
  Serial.println("Initializing GSM...");
  
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while(1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); Serial.println(imei);
  }

  fonaSerial->print("AT+CNMI=2,1\r\n");
}

void deleteAllSms() {
  int8_t smsnum = fona.getNumSMS();
  if (smsnum < 0) {
    Serial.println(F("No SMS found!"));
  } else {
    Serial.print(smsnum);
    Serial.println(F("Deleting all sms..."));
    for (int i = 0; i < smsnum; i++) {
      int smsNumTemp = i + 1;
      if (fona.deleteSMS(smsNumTemp)) {
        Serial.println(F("OK!"));
      } else {
        Serial.println(F("Couldn't delete"));
      }
    }
    Serial.println(F("DONE!"));
  }
}

void smsListener() {
  char* bufPtr = fonaNotificationBuffer;    //handy buffer pointer
  
  if (fona.available()) {
    int slot = 0;
    int charCount = 0;
    
    do  {
      *bufPtr = fona.read();
      Serial.write(*bufPtr);
      delay(1);
    } while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (sizeof(fonaNotificationBuffer)-1)));
    
    *bufPtr = 0;

    if (1 == sscanf(fonaNotificationBuffer, "+CMTI: " FONA_PREF_SMS_STORAGE ",%d", &slot)) {
      Serial.print("slot: "); Serial.println(slot);
      
      char callerIDbuffer[32];  //we'll store the SMS sender number in here
      
      // Retrieve SMS sender address/phone number.
      if (! fona.getSMSSender(slot, callerIDbuffer, 31)) {
        Serial.println("Didn't find SMS message in slot!");
      }
      Serial.print(F("FROM: ")); Serial.println(callerIDbuffer);

        // Retrieve SMS value.
        uint16_t smslen;
        if (fona.readSMS(slot, smsBuffer, 250, &smslen)) { // pass in buffer and max len!
          Serial.println(smsBuffer);
        }

      //Send back an automatic response
      Serial.println("Sending reponse...");
      if (!fona.sendSMS(callerIDbuffer, "Hey, I got your text!")) {
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("Sent!"));
      }

      deleteAllSms();
    }
  }
}
