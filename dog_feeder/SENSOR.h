#ifndef SENSOR_h
#define SENSOR_h

#include <Arduino.h>

// uncomment to see debug values
//#define DEBUG_SENSOR

#define feeder1SensorToBottom 24
#define feeder1SensorToTop 17

#define mainSensorToBottom 24
#define mainSensorToTop 17

class SENSOR
{
  public:
    void SENSOR_setup(int trig1, int echo1, int trig2, int echo2);
    void updateSensorValueMain();
    void updateSensorValueBowl1();
    int getTank1PercentLeft();
    int getMainTankPercentLeft();
  private:
    // feeder 1 sensor
    int echoPin1;
    int trigPin1;
    long duration1;
    int distance1;

    // feeds tank
    int echoPin2;
    int trigPin2;
    long duration2;
    int distance2;
};

#endif
