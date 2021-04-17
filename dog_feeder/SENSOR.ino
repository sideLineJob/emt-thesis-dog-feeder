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

void SENSOR::getSensorValueMain() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  #ifdef DEBUG
    Serial.print("Distance 1: ");
    Serial.print(distance1);
    Serial.println(" cm");
  #endif
}

void SENSOR::getSensorValueBowl1() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  #ifdef DEBUG
    Serial.print("Distance 2: ");
    Serial.print(distance2);
    Serial.println(" cm");
  #endif
}
