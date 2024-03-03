//
// Created by rico on 1/4/2023.
//

#ifndef MULTISENSORDDRPAD_ARROWPANEL_H
#define MULTISENSORDDRPAD_ARROWPANEL_H

#include <cstdint>

class ArrowPanel
{
public:
    uint8_t NeopixelPin0 = 0;
    uint8_t NeopixelPin1 = 0;
    uint8_t NeopixelPin2 = 0;
    uint8_t NeopixelPin3 = 0;
    uint8_t ButtonId = 0;

    ArrowPanel(uint8_t np0, uint8_t np1, uint8_t np2, uint8_t np3, uint8_t buttonId)
    {
        NeopixelPin0 = np0;
        NeopixelPin1 = np1;
        NeopixelPin2 = np2;
        NeopixelPin3 = np3;
        ButtonId = buttonId;
    }

    void Evaluate(float average)
    {
        bool isArrowPressed = average > threshold;

        int lightBrightness = 0;

        if(isArrowPressed)
        {
            lightBrightness = BRIGHTNESS_WHITE;
        }

        uint32_t rgbcolor = neopixelStrip.ColorHSV(0, 0, lightBrightness);
        neopixelStrip.setPixelColor(NeopixelPin0, rgbcolor);
        neopixelStrip.setPixelColor(NeopixelPin1, rgbcolor);
        neopixelStrip.setPixelColor(NeopixelPin2, rgbcolor);
        neopixelStrip.setPixelColor(NeopixelPin3, rgbcolor);

        if(bleGamepad.isConnected())
        {
            if(isArrowPressed)
            {
                bleGamepad.press(ButtonId+1);
            }
            else
            {
                bleGamepad.release(ButtonId+1);
            }
        }
        //Joystick.setButton(ButtonId, isArrowPressed);
    }
};


#endif //MULTISENSORDDRPAD_ARROWPANEL_H
