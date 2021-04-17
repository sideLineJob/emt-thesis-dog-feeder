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

void TIME::isMorning() {
  Serial.print("Year: ");
  Serial.println(yearValue);
  Serial.print("Month: ");
  Serial.println(monthValue);
  Serial.print("Date: ");
  Serial.println(dateValue);
  Serial.print("Hour: ");
  Serial.println(hourValue);
  Serial.print("Minute: ");
  Serial.println(minuteValue);
  Serial.print("Second: ");
  Serial.println(secondValue);
  Serial.println("------------");
  delay(500);
}
