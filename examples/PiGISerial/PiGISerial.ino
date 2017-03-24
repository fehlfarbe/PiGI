#include <PiGI.h>

#define LED 8
#define BUZZER 7

// Setup PiGI #1 on digital Pin D3 and SBM-20 GM tube conversion factor and deadtime
PiGI pigi(3, PIGI1, 0.00277, 190);

void setup() {
    // setup Serial
    Serial.begin(9600);
    Serial.println("PiGI");

    // setup pins for LED and buzzer
    pinMode(LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
}

void loop() {
    Serial.print("CPM: ");
    Serial.print(pigi.cpm());
    Serial.print(" ==> ");
    Serial.print(pigi.uSvh());
    Serial.println(" uSv/h");

    pigi.blinkAndBeep(LED, BUZZER);
    delay(100);
}
