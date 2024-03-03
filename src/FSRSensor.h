//
// Created by rico on 1/4/2023.
//

#ifndef MULTISENSORDDRPAD_FSRSENSOR_H
#define MULTISENSORDDRPAD_FSRSENSOR_H

#include "Multiplexer.h"
#include <Arduino.h>

class FSRSensor
{
public:
    Multiplexer ParentMultiplexer;
    uint8_t MultiplexerPin;
    String SensorName;
    uint8_t Arrow;
    uint8_t Corner = 0;
    uint8_t NeopixelPin = 0;
    uint8_t CurrentRawValue = 0;
    uint8_t AmbientReadings[READINGS_COUNT];
    uint8_t AmbientValue = 0;
    uint8_t AmbientDelta = 0;
    float CurrentRatio = 0;

    FSRSensor(Multiplexer parent, int pin, String sensorName, int arrow, int corner, int npPin)
    {
        ParentMultiplexer = parent;
        MultiplexerPin = pin;
        SensorName = sensorName;
        Arrow = arrow;
        Corner = corner;
        NeopixelPin = npPin;
        if(VERBOSE_OUTPUT)
        {
            Serial.println("INITIALIZED: " + SensorName + (String)Arrow  + "(" + (String)Corner + ")");
        }
    }

    void SetAmbientValue()
    {
        if(VERBOSE_OUTPUT)
        {      Serial.println("AMBIENT VALUE SETTING");
        }

        uint16_t sum = 0;

        for(int i = 0; i < READINGS_COUNT; i++)
        {
            sum += AmbientReadings[i];
        }

        AmbientValue = (uint8_t)(sum / READINGS_COUNT);
        if(VERBOSE_OUTPUT)
        {
            Serial.println("AMBIENT VALUE FOR " + SensorName + ": " + (String)AmbientValue);
        }

        AmbientDelta = 255-AmbientValue;
    }

    float EvaluateCurrentState()
    {
        CurrentRawValue = ParentMultiplexer.EvaluateKeypress(MultiplexerPin);


        uint8_t floorValue = CurrentRawValue - AmbientValue;
        if(CurrentRawValue < AmbientValue)
        {
            floorValue = 0;
        }

        float ratio = ((float)floorValue / (float)AmbientDelta);

        if(ratio > 1)
        {
            ratio = 1;
        }

        if(ratio < 0)
        {
            ratio = 0;
        }

        CurrentRatio = ratio;

        /*
        Serial.println(SensorName + "CurrentRawValue : " + (String)CurrentRawValue
                                  + ", AmbientValue : " + (String)AmbientValue
                                  + ", floorValue : " + (String)floorValue
                                  + ", ratio : " + (String)ratio
                                  + ", CurrentRatio : " + (String)CurrentRatio); 

                                  */


        SetLighting();

        return CurrentRatio;
    }

    uint8_t CurrentValue()
    {
        return CurrentRawValue;
    }

    void SetLighting()
    {
        int lightBrightness = CurrentRatio * 255;
        int hue = 0;

        if(Arrow == 0 || Arrow == 3)
        {
            hue = BLUE_HUE;
        }
        else
        {
            hue = RED_HUE;
        }

        // hue is 0 to 65535
        // sat and val are 0 to 255
        uint32_t rgbcolor = neopixelStrip.ColorHSV(hue, 255, lightBrightness);
        neopixelStrip.setPixelColor(NeopixelPin, rgbcolor);
    }
};


#endif //MULTISENSORDDRPAD_FSRSENSOR_H
