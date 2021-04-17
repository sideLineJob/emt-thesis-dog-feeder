#include <Arduino.h>
#include "RELAY.h"

void RELAY::RELAY_setup(int in1, int in2, int in3, int in4) {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);

  relayInput1 = in1;
  relayInput2 = in2;
  relayInput3 = in3;
  relayInput4 = in4;
}

void RELAY::feederShow() {
  digitalWrite(relayInput1, LOW);
  digitalWrite(relayInput2, HIGH);
  digitalWrite(relayInput3, LOW);
  digitalWrite(relayInput4, HIGH);
}

void RELAY::feederHide() {
  digitalWrite(relayInput1, HIGH);
  digitalWrite(relayInput2, LOW);
  digitalWrite(relayInput3, HIGH);
  digitalWrite(relayInput4, LOW);
}
