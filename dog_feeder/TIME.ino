#include "TIME.h"

void TIME::TIME_setup() {
  Wire.begin();
}

void TIME::updateTime() {
  yearValue = (clock.getYear(), DEC);
  monthValue = (clock.getMonth(century), DEC);
  dateValue = (clock.getDate(), DEC);
  hourValue = (clock.getHour(h12Flag, pmFlag), DEC);
  minuteValue = (clock.getMinute(), DEC);
  secondValue = (clock.getSecond(), DEC);

  #ifdef DEBUG_TIME
    Serial.print(yearValue); Serial.print("-");
    Serial.print(monthValue); Serial.print("-");
    Serial.print(dateValue); Serial.print(" ");
    Serial.print(hourValue); Serial.print(":"); //24-hr
    Serial.print(minuteValue); Serial.print(":");
    Serial.println(secondValue);
    delay(500);
  #endif
}
