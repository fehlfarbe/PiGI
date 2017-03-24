#ifndef PIGI_PIGI_H
#define PIGI_PIGI_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Enumeration for multiple PiGI setups and ISR setup
enum ePiGINum {
    PIGI1 = 0,
    PIGI2,
    _PIGI_MAX
};

class PiGI {
public:
    PiGI(int pin, ePiGINum num, double tube_conv=1.0, int tube_deadtime_ns=0);

    long counts();
    float cpm();
    double cps();
    double uSvh();
    double corrected_rate();

    void blinkAndBeep(int led_pin = -1, int beep_pin = -1);

    // should be protected, but ISR can't access if public
    void __count();

protected:
    // PiGI number
    int m_num;
    // PiGI interrupt pin
    int m_pin;

    // tube conversion factor for GM tube,
    // see: https://apollo.open-resource.org/lab:pigi:software#convert-cpm-to-usv-h
    double m_tube_conv = 1.0;
    // tube deadtime in us for GM tube,
    // see: https://apollo.open-resource.org/lab:pigi:software#effect-of-dead-time-on-counts
    int m_tube_deadtime = 0;

    // for CPM calculation
    long m_lasttime = 0;
    float m_cpm = 0;
    // integration time for CPM in ms (default: 10s)
    const long m_const_countingtime = 10000;

    // counters
    volatile long m_counter = 0;
    volatile long m_counter_cpm = 0;
    long m_lastblinkcounter = 0;
};


#endif //PIGI_PIGI_H
