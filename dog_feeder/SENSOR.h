#ifndef SENSOR_h
#define SENSOR_h

#include <Arduino.h>

// uncomment to see debug values
//#define DEBUG_SENSOR

class SENSOR
{
  public:
    void SENSOR_setup(int trig1, int echo1, int trig2, int echo2);
    void updateSensorValueMain();
    void updateSensorValueBowl1();
  private:
    int echoPin1;
    int trigPin1;
    long duration1;
    int distance1;
    int echoPin2;
    int trigPin2;
    long duration2;
    int distance2;
};

#endif
