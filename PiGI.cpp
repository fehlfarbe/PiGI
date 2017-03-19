//
// Created by kolbe on 15.03.17.
//

#include "PiGI.h"

PiGI *__PIGIS[_PIGI_MAX];


void ISR_PIGI1(){
    if(__PIGIS[PIGI1] != 0){
        __PIGIS[PIGI1]->count();
    }
}

void ISR_PIGI2(){
    if(__PIGIS[PIGI2] != 0){
        __PIGIS[PIGI2]->count();
    }
}

long PiGI::counts() {
    return m_counter;
}

PiGI::PiGI(int pin, ePiGINum num) {
    m_pin = pin;
    m_num = num;
    __PIGIS[num] = this;

    pinMode(m_pin, INPUT_PULLUP);
    switch (m_num){
        case PIGI1:
            attachInterrupt(digitalPinToInterrupt(m_pin), ISR_PIGI1, FALLING);
            break;
        case PIGI2:
            attachInterrupt(digitalPinToInterrupt(m_pin), ISR_PIGI2, FALLING);
            break;
    }
}

void PiGI::count() {
    m_counter++;
}

float PiGI::cpm() {
    long current = millis();
    long diff = current - m_lasttime;

    if(diff > m_const_countingtime){
        m_lasttime = current;
        m_cpm = (float)m_counter / diff * 60000;
        m_counter = m_lastblinkcounter = m_lastbeepcounter = 0;
    }

    return m_cpm;
}

void PiGI::blinkAndBeep(int led_pin, int beep_pin) {
    if(m_counter > m_lastblinkcounter){
        if( led_pin > 0){
            digitalWrite(led_pin, HIGH);
            delay(1);
            digitalWrite(led_pin, LOW);
        }
        if( beep_pin> 0){
            digitalWrite(beep_pin, HIGH);
            delay(1);
            digitalWrite(beep_pin, LOW);
        }

        m_lastblinkcounter = m_counter;
    }
}

