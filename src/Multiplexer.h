//
// Created by rico on 1/4/2023.
//

#ifndef MULTISENSORDDRPAD_MULTIPLEXER_H
#define MULTISENSORDDRPAD_MULTIPLEXER_H

#include <Arduino.h>
#include <cstdint>

class Multiplexer
{
public:
    int InputPin0 = 0;
    int InputPin1 = 0;
    int InputPin2 = 0;
    int InputPin3 = 0;
    int SignalPin = 0;

    Multiplexer()
    {

    }

    Multiplexer(int inPin0, int inPin1, int inPin2, int inPin3, int sigPin)
    {
        InputPin0 = inPin0;
        InputPin1 = inPin1;
        InputPin2 = inPin2;
        InputPin3 = inPin3;
        SignalPin = sigPin;
    }

    void InitializePins()
    {
        pinMode(InputPin0, OUTPUT);
        pinMode(InputPin1, OUTPUT);
        pinMode(InputPin2, OUTPUT);
        pinMode(InputPin3, OUTPUT);
        pinMode(SignalPin, INPUT);
    }

    uint8_t EvaluateKeypress(int pinToTest)
    {
        switch (pinToTest) {
            case 0:
                digitalWrite(InputPin0, false);
                digitalWrite(InputPin1, false);
                digitalWrite(InputPin2, false);
                digitalWrite(InputPin3, false);
                break;
            case 1:
                digitalWrite(InputPin0, true);
                digitalWrite(InputPin1, false);
                digitalWrite(InputPin2, false);
                digitalWrite(InputPin3, false);
                break;
            case 2:
                digitalWrite(InputPin0, false);
                digitalWrite(InputPin1, true);
                digitalWrite(InputPin2, false);
                digitalWrite(InputPin3, false);
                break;
            case 3:
                digitalWrite(InputPin0, true);
                digitalWrite(InputPin1, true);
                digitalWrite(InputPin2, false);
                digitalWrite(InputPin3, false);
                break;
            case 4:
                digitalWrite(InputPin0, false);
                digitalWrite(InputPin1, false);
                digitalWrite(InputPin2, true);
                digitalWrite(InputPin3, false);
                break;
            case 5:
                digitalWrite(InputPin0, true);
                digitalWrite(InputPin1, false);
                digitalWrite(InputPin2, true);
                digitalWrite(InputPin3, false);
                break;
            case 6:
                digitalWrite(InputPin0, false);
                digitalWrite(InputPin1, true);
                digitalWrite(InputPin2, true);
                digitalWrite(InputPin3, false);
                break;
            case 7:
                digitalWrite(InputPin0, true);
                digitalWrite(InputPin1, true);
                digitalWrite(InputPin2, true);
                digitalWrite(InputPin3, false);
                break;
            case 8:
                digitalWrite(InputPin0, false);
                digitalWrite(InputPin1, false);
                digitalWrite(InputPin2, false);
                digitalWrite(InputPin3, true);
                break;
            case 9:
                digitalWrite(InputPin0, true);
                digitalWrite(InputPin1, false);
                digitalWrite(InputPin2, false);
                digitalWrite(InputPin3, true);
                break;
            case 10:
                digitalWrite(InputPin0, false);
                digitalWrite(InputPin1, true);
                digitalWrite(InputPin2, false);
                digitalWrite(InputPin3, true);
                break;
            case 11:
                digitalWrite(InputPin0, true);
                digitalWrite(InputPin1, true);
                digitalWrite(InputPin2, false);
                digitalWrite(InputPin3, true);
                break;
            case 12:
                digitalWrite(InputPin0, false);
                digitalWrite(InputPin1, false);
                digitalWrite(InputPin2, true);
                digitalWrite(InputPin3, true);
                break;
            case 13:
                digitalWrite(InputPin0, true);
                digitalWrite(InputPin1, false);
                digitalWrite(InputPin2, true);
                digitalWrite(InputPin3, true);
                break;
            case 14:
                digitalWrite(InputPin0, false);
                digitalWrite(InputPin1, true);
                digitalWrite(InputPin2, true);
                digitalWrite(InputPin3, true);
                break;
            case 15:
                digitalWrite(InputPin0, true);
                digitalWrite(InputPin1, true);
                digitalWrite(InputPin2, true);
                digitalWrite(InputPin3, true);
                break;
            default:
                break;
        }

        delayMicroseconds(10);

        return((uint8_t)(analogRead(SignalPin)/16));
    }
};

#endif //MULTISENSORDDRPAD_MULTIPLEXER_H
