#include <Arduino.h>
#include "SENSOR.h"

void SENSOR::SENSOR_setup(int trig1, int echo1, int trig2, int echo2) {
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  echoPin1 = echo1;
  trigPin1 = trig1;
  echoPin2 = echo2;
  trigPin2 = trig2;
}

void SENSOR::updateSensorValueMain() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  #ifdef DEBUG_SENSOR
    Serial.print("Distance 1: ");
    Serial.print(distance1);
    Serial.println(" cm");
  #endif
}

void SENSOR::updateSensorValueBowl1() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  #ifdef DEBUG_SENSOR
    Serial.print("Distance 2: ");
    Serial.print(distance2);
    Serial.println(" cm");
  #endif
}

int SENSOR::getTank1PercentLeft() {
  int tankHeight = feeder1SensorToBottom - feeder1SensorToTop;

  int tankLeft = feeder1SensorToBottom - distance1;

  int leftInPercent = (100 * tankLeft) / tankHeight;

//  Serial.print("Tank1 (x% left): ");
//  Serial.print(leftInPercent < 0 ? (leftInPercent = leftInPercent * -1) : leftInPercent);
//  Serial.println("%");
//  Serial.print("Tank Height (T2): ");
//  Serial.println(tankHeight);
//  Serial.print("Tank Left (x): ");
//  Serial.println(tankLeft);
//  Serial.print("Distance(y): ");
//  Serial.println(distance1);
//  Serial.println("\n\n---\n\n");
//  delay(100);

  return leftInPercent;
}


int SENSOR::getMainTankPercentLeft() {
  int tankHeight = mainSensorToBottom - mainSensorToTop;

  int tankLeft = mainSensorToBottom - distance2;

  int leftInPercent = (100 * tankLeft) / tankHeight;

  Serial.print("MainTank: ");
  Serial.println(leftInPercent);

  Serial.print("Tank1 (x% left): ");
  Serial.print(leftInPercent < 0 ? (leftInPercent = leftInPercent * -1) : leftInPercent);
  Serial.println("%");
  Serial.print("Tank Height (T2): ");
  Serial.println(tankHeight);
  Serial.print("Tank Left (x): ");
  Serial.println(tankLeft);
  Serial.print("Distance(y): ");
  Serial.println(distance2);
  Serial.println("\n\n---\n\n");
  delay(100);
  
  return leftInPercent;
}
