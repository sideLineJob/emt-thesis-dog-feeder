#ifndef RELAY_h
#define RELAY_h

#include <Arduino.h>

// uncomment to see debug values
//#define DEBUG_SENSOR

class RELAY
{
  public:
    void RELAY_setup(int in1, int in2, int in3, int in4);
    void feederShow();
    void feederHide();

  private:
    int relayInput1;
    int relayInput2;
    int relayInput3;
    int relayInput4;    
};

#endif
