//
// Created by kolbe on 15.03.17.
//

#ifndef PIGI_PIGI_H
#define PIGI_PIGI_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


enum ePiGINum {
    PIGI1 = 0,
    PIGI2,
    _PIGI_MAX
};

class PiGI {
public:
    PiGI(int pin, ePiGINum num);

    long counts();
    float cpm();

    void blinkAndBeep(int led_pin = -1, int beep_pin = -1);

    // should be protected or so?!
    void count();

protected:
    int m_num;
    int m_pin;
    const long m_const_countingtime = 10000;    // couning time for CPM in ms (default: 10s)

    long m_lasttime = 0;

    float m_cpm = 0;
    
    volatile long m_counter = 0;
    long m_lastbeepcounter = 0;
    long m_lastblinkcounter = 0;
};


#endif //PIGI_PIGI_H
