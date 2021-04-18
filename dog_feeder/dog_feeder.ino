#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>
#include "SENSOR.h"
#include "TIME.h"
#include "RELAY.h"
#include "FEEDER_CMD.h"
#include <Servo.h>

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

/**
 * RELAY config
 */
RELAY switchCont;

/**
 * CMDs
 */
FEEDER_CMD cmdConst;
String cmd = "";
boolean cmdAvail = false;

#define IN1 42
#define IN2 43
#define IN3 44
#define IN4 45

boolean sendUpdatedTank = false;
boolean feederShowedState = false;

/**
 * Servo Controller
 */
Servo containerServo;

void setup() {
  while (!Serial);
  Serial.begin(115200);
  
  containerServo.attach(8);
  
  GSM_setup();
  sensorCont.SENSOR_setup(TRIG_PIN1, ECHO_PIN1, TRIG_PIN2, ECHO_PIN2);
  timeCont.TIME_setup();
  switchCont.RELAY_setup(IN1, IN2, IN3, IN4);  

  /**
   * Initially hide feeder
   */
  switchCont.feederHide();
}

  
char fonaNotificationBuffer[64];
char smsBuffer[250];
char callerIDbuffer[32] = "+639068936643";  //we'll store the SMS sender number in here


void loop() {
  smsListener();
  sensorCont.updateSensorValueMain();
  sensorCont.updateSensorValueBowl1();
  timeCont.updateTime();
  cmdListener();

//  sensorCont.getTank1PercentLeft();
  sensorCont.getMainTankPercentLeft();

  if (sendUpdatedTank) {
    int tank1Left = sensorCont.getTank1PercentLeft();
    int mainTankLeft = sensorCont.getMainTankPercentLeft();

    if (tank1Left < 0) {
      tank1Left = tank1Left * -1;
    }

    if (mainTankLeft < 0) {
      mainTankLeft = mainTankLeft * -1;
    }

    sendUpdatedTank = false;
    Serial.println("Sending updated tank...");

    String sms = "Main Tank Left: " + String(mainTankLeft) + "%\nFeeder Container Left: " + String(tank1Left) + "%";
    int smsLen = sms.length() + 1;
    char smsCharArr[smsLen];
    sms.toCharArray(smsCharArr, smsLen);
    
    if (!fona.sendSMS(callerIDbuffer, smsCharArr)) {
        Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }
  }

 
//  timeCont.isMorning();
  
  if (Serial.available() > 0) {
    String cmdTemp = Serial.readString();
    cmd = cmdTemp;
    cmdAvail = true;
  }
}

/**
 * CMD Listener & Controller
 */
void cmdListener() {
  if (cmdAvail == true) {
    cmdController();

    cmd = "";
    cmdAvail = false;
  }
}

void cmdController() {
  if (cmd.indexOf(cmdConst.SHOW_FEEDER_1) >= 0 && feederShowedState == false) {

      // open main container
      Serial.println("Main container opened!");
      openContainer();
      delay(10000);
      Serial.println("Main container closed!");

      delay(1000);
      Serial.println("Extending feeder container...");
      switchCont.feederShow();
      delay(5000);
      Serial.println("Feeder container extended!");
      delay(1000);
      Serial.println("Sending sms response...");
      
      if (!fona.sendSMS(callerIDbuffer, "Feeder showed successfully!")) {
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("Sent!"));
      }

      feederShowedState = true;

  } else if (cmd.indexOf(cmdConst.SHOW_FEEDER_1) >= 0 && feederShowedState == true) {
      if (!fona.sendSMS(callerIDbuffer, "Feeder container already extended!")) {
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("Sent!"));
      }
      
  } else if (cmd.indexOf(cmdConst.HIDE_FEEDER_1) >= 0) {
      Serial.println("Retracting feeder container...");
      switchCont.feederHide();
      delay(5000);
      Serial.println("Feeder container retracted!");
      delay(1000);
      Serial.println("Sending sms response...");

      if (!fona.sendSMS(callerIDbuffer, "Feeder hide successfully!")) {
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("Sent!"));
      }

      sendUpdatedTank = true;
      feederShowedState = false;
  }
}

/**
 * Servo Controller
 */
void closeContainer() {
  containerServo.write(90);
  delay(15);
}

void openContainer() {
  containerServo.write(2);
  delay(15);
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
      
      // Retrieve SMS sender address/phone number.
      if (! fona.getSMSSender(slot, callerIDbuffer, 31)) {
        Serial.println("Didn't find SMS message in slot!");
      }
      Serial.print(F("FROM: ")); Serial.println(callerIDbuffer);

        // Retrieve SMS value.
        uint16_t smslen;
        if (fona.readSMS(slot, smsBuffer, 250, &smslen)) { // pass in buffer and max len!
          Serial.print("Serial buffer: ");
          Serial.println(smsBuffer);
          cmd = smsBuffer;
          cmdAvail = true;
        }

      deleteAllSms();
    }
  }
}
