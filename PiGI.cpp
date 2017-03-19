#include "PiGI.h"

// Object references for ISR
PiGI *__PIGIS[_PIGI_MAX];

// ISR for PiGI event counting
// @todo: is there a better, more dynamic solution for ISR in classes?
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

/**
 * Number of events. Resets after CPM recalculation.
 * @return
 */
long PiGI::counts() {
    return m_counter;
}

/**
 *
 * @param pin PiGI signal pin
 * @param num Attach it to PiGI interrupt 1 or 2
 */
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

/**
 * Increments internal event counter (used by PiGI's ISR)
 */
void PiGI::count() {
    m_counter++;
}

/**
 * Returns CPM (counts per minute). The CPM are updated every 10 seconds
 * @return CPM
 */
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

/**
 * Blinks or beeps if there were events since the last call.
 * Just write -1 if you don't need both pins.
 * @param led_pin LED pin number (-1 for no LED)
 * @param beep_pin Buzzer pin number (-1 for no buzzer)
 */
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

