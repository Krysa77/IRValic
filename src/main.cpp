#include <Arduino.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 12 // pin pro prijimac
#define ENTER 4127850240 // enter klavesa
#define redLed 5 // cervena
#define greenLed 6 // zelena
#define blueLed 9 // modra

unsigned long codes[] = { //kody pro cisla 0-9
    3910598400, // 0
    4077715200, // 1
    3877175040, // 2
    2707357440, // 3
    4144561920, // 4
    3810328320, // 5
    2774204160, // 6
    3175284480, // 7
    2907897600, // 8
    3041591040, // 9
};

String stringNum = "";
int currentLED = 0; // tedka ledka

int codesSize = sizeof(codes) / sizeof(codes[0]); // zjisit velikost pole

String receivedNum(unsigned long irData) {
    for (int i = 0; i < codesSize; i++) { // Kontroluje kody
        if (irData == codes[i]) {
            return String(i);
        }
    }
    return "";
}

void setLED(int value) { // vraceni intenzity led
    switch (currentLED) { //podle ledky
        case 0:
            analogWrite(redLed, value);
            Serial.print("Cervena LED: ");
            Serial.println(value);
            break;
        case 1:
            analogWrite(greenLed, value);
            Serial.print("Zelena LED: ");
            Serial.println(value);
            break;
        case 2:
            analogWrite(blueLed, value);
            Serial.print("Modra LED: ");
            Serial.println(value);
            break;
    }
}

void setup() {
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // inicializace prijimace
    Serial.begin(9600); // inicializace seriove komunikace
    //Inicializace ledek
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
}

void loop() {
    if (IrReceiver.decode()) {
        unsigned long irData = IrReceiver.decodedIRData.decodedRawData; // ulozit prijaty kod
        if (irData > 0) { 
            Serial.print(irData); // vypis
            Serial.print(",");
            if (irData == ENTER) { // if enter -> neco lol
                int intNum = stringNum.toInt(); // string to int
                if (intNum >= 0 && intNum <= 255) { // rozsah kontrola
                    setLED(intNum);
                    Serial.print("Nastavena hodnota: ");
                    Serial.println(intNum); // vypis cisla zas
                    stringNum = "";
                } else {
                    Serial.println("Mimo rozsah");
                    stringNum = "";
                }
            } else if (irData == 3125149440) { // cervena led
                currentLED = 0;
                Serial.println("Vybrana cervena LED");
            } else if (irData == 3108437760) { // zelana led
                currentLED = 1;
                Serial.println("Vybrana zelena LED");
            } else if (irData == 3091726080) { // modra led
                currentLED = 2;
                Serial.println("Vybrana modra LED");
            } else { 
                stringNum += receivedNum(irData);
                Serial.println(stringNum); // vypis cisla
            }
        }

        IrReceiver.resume(); // poveleni prijmlu dasiho kodu / resume receiveru
    }
}
