#ifndef TIME_h
#define TIME_h

#include <Arduino.h>
#include <DS3231.h>
#include <Wire.h>

DS3231 clock;

// uncomment to see debug values
#define DEBUG_TIME

class TIME {
  public:
    void TIME_setup();
    void updateTime();
  private:
    bool century = false;
    bool h12Flag;
    bool pmFlag;
    int yearValue;
    int monthValue;
    int dateValue;
    int hourValue;
    int minuteValue;
    int secondValue;
};

#endif
