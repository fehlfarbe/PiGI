#include "PiGI.h"

// Object references for ISR
PiGI *__PIGIS[_PIGI_MAX];

// ISR for PiGI event counting
// @todo: is there a better, more dynamic solution for multiple ISR in classes?
void ISR_PIGI1(){
    if(__PIGIS[PIGI1] != 0){
        __PIGIS[PIGI1]->__count();
    }
}

void ISR_PIGI2(){
    if(__PIGIS[PIGI2] != 0){
        __PIGIS[PIGI2]->__count();
    }
}

/**
 *
 * @param pin PiGI signal pin
 * @param num Attach it to PiGI interrupt 1 or 2
 * @param tube_conv GM tube conversion factor
 * @param tube_deadtime GM tube deadtime in us
 */
PiGI::PiGI(int pin, ePiGINum num, double tube_conv, int tube_deadtime_ns) {
    m_pin = pin;
    m_num = num;
    m_tube_conv = tube_conv;
    m_tube_deadtime = tube_deadtime_ns;
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
void PiGI::__count() {
    m_counter++;
    m_counter_cpm++;
}

/**
 * CPM (counts per minute). The CPM are updated every 10 seconds
 * @return CPM
 */
float PiGI::cpm() {
    long current = millis();
    long diff = current - m_lasttime;

    if(diff > m_const_countingtime){
        m_lasttime = current;
        m_cpm = (float)m_counter_cpm / diff * 60000;
        m_counter_cpm = m_lastblinkcounter = 0;
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
    if(m_counter_cpm > m_lastblinkcounter){
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

        m_lastblinkcounter = m_counter_cpm;
    }
}


/**
 * Total number of events
 * @return
 */
long PiGI::counts() {
    return m_counter;
}

/**
 * Current radiation dose in uSv/h
 * @return
 */
double PiGI::uSvh() {
    return corrected_rate()*m_tube_conv;
}

/**
 * Counts per second
 * @return
 */
double PiGI::cps() {
    return cpm() / 60.0;
}

/**
 * GM tube deadtime corrected counting rate
 * @return
 */
double PiGI::corrected_rate() {
    double rate = cpm();
    return rate / (1 - rate*(m_tube_deadtime/1000000.0));
}