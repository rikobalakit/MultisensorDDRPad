#include "main.h"

Multiplexer Multiplexer0(PIN_OUT_S0, PIN_OUT_S1, PIN_OUT_S2, PIN_OUT_S3, PIN_IN_SENSOR);

//blue red green black
//FSRSensor(Multiplexer parent, int pin, String sensorName, int arrow, int corner, int npPin   )
FSRSensor LeftTL(Multiplexer0, 15, "LeftTL", 0, 0, 24); //blue
FSRSensor LeftBL(Multiplexer0, 14, "LeftBL", 0, 1, 32); //red
FSRSensor LeftBR(Multiplexer0, 13, "LeftBR", 0, 2, 33); //green
FSRSensor LeftTR(Multiplexer0, 12, "LeftTR", 0, 3, 25); //black

FSRSensor DownTL(Multiplexer0, 11, "DownTL", 1, 0, 51); //blue
FSRSensor DownBL(Multiplexer0, 10, "DownBL", 1, 1, 59); //red
FSRSensor DownBR(Multiplexer0, 9, "DownBR", 1, 2, 60); //green
FSRSensor DownTR(Multiplexer0, 8, "DownTR", 1, 3, 52); //black

FSRSensor UpTL(Multiplexer0, 7, "UpTL", 2, 0, 3); //blue
FSRSensor UpBL(Multiplexer0, 6, "UpBL", 2, 1, 11); //red
FSRSensor UpBR(Multiplexer0, 5, "UpBR", 2, 2, 12); //green
FSRSensor UpTR(Multiplexer0, 4, "UpTR", 2, 3, 4); //black

FSRSensor RightTL(Multiplexer0, 3, "RightTL", 3, 0, 30); //blue
FSRSensor RightBL(Multiplexer0, 2, "RightBL", 3, 1, 38); //red
FSRSensor RightBR(Multiplexer0, 1, "RightBR", 3, 2, 39); //green
FSRSensor RightTR(Multiplexer0, 0, "RightTR", 3, 3, 31); //black

ArrowPanel LeftPanel(16, 17, 40, 41, 0);
ArrowPanel DownPanel(50, 53, 58, 61, 1);
ArrowPanel UpPanel(2, 5, 10, 13, 2);
ArrowPanel RightPanel(22, 23, 46, 47, 3);

float CurrentStates[16];

FSRSensor AllSensors[16] = {LeftTL, LeftBL, LeftBR, LeftTR, DownTL, DownBL, DownBR, DownTR, UpTL, UpBL, UpBR, UpTR, RightTL, RightBL, RightBR, RightTR};
ArrowPanel AllArrows[4] = {LeftPanel, DownPanel, UpPanel, RightPanel};

void setup()
{
    Serial.begin(19200);

    BleGamepadConfiguration bleGamepadConfig;
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setButtonCount(4);
    bleGamepad.begin(&bleGamepadConfig);
    
    pinMode(PIN_IN_KNOB_SENSITIVITY, INPUT);
    pinMode(PIN_IN_BUTTON_RESET, INPUT_PULLDOWN);
    //Joystick.begin();
    Multiplexer0.InitializePins();
    neopixelStrip.begin();

    // put your setup code here, to run once:
}

void SetInitializationReading(int index)
{
    for(int i = 0; i < TOTAL_FSRS; i++)
    {
        AllSensors[i].EvaluateCurrentState();
        AllSensors[i].AmbientReadings[index] = AllSensors[i].CurrentRawValue;
    }
}

void SetAmbientAverage()
{
    for(int i = 0; i < TOTAL_FSRS; i++)
    {
        AllSensors[i].SetAmbientValue();
    }
}


void EvaluateAllKeys()
{
    for(int i = 0; i < TOTAL_FSRS; i++)
    {
        CurrentStates[i] = AllSensors[i].EvaluateCurrentState();
    }
}

void EvaluateArrows()
{
    float leftAverage = (CurrentStates[0]+CurrentStates[1]+CurrentStates[2]+CurrentStates[3])/4;
    float downAverage = (CurrentStates[4]+CurrentStates[5]+CurrentStates[6]+CurrentStates[7])/4;
    float upAverage = (CurrentStates[8]+CurrentStates[9]+CurrentStates[10]+CurrentStates[11])/4;
    float rightAverage = (CurrentStates[12]+CurrentStates[13]+CurrentStates[14]+CurrentStates[15])/4;

    LeftPanel.Evaluate(leftAverage);
    DownPanel.Evaluate(downAverage);
    UpPanel.Evaluate(upAverage);
    RightPanel.Evaluate(rightAverage);
}

void ShowNeopixels()
{
    neopixelStrip.show();
}

void ReadKnobAverage(bool forceReset)
{
    KnobReadings[knobReadingIndex] = analogRead(PIN_IN_KNOB_SENSITIVITY) / 16;

    knobReadingIndex++;

    if(knobReadingIndex >= KNOB_READINGS_COUNT)
    {
        knobReadingIndex = 0;
    }

    uint32_t total = 0;
    for(int i = 0; i < KNOB_READINGS_COUNT; i++)
    {
        total += KnobReadings[i];
    }

    uint8_t currentAverage = total / KNOB_READINGS_COUNT;

    if((currentAverage - KnobReadingAverage > 8 || currentAverage - KnobReadingAverage < -8) || forceReset)
    {

        KnobReadingAverage = total / KNOB_READINGS_COUNT;


        float unclampedThreshold = -endCutoff+((float)KnobReadingAverage/(float)255)*(1+endCutoff*2);
        if(unclampedThreshold > thresholdMaximum)
        {
            unclampedThreshold = thresholdMaximum;
        }

        if(unclampedThreshold < thresholdMinimum)
        {
            unclampedThreshold = thresholdMinimum;
        }

        threshold = unclampedThreshold;

        KnobChangeTimeout = KnobChangeDisplayLengthTicks;
    }



}

void DisplaySensitivityThreshold()
{
    if(KnobChangeTimeout < 1)
    {
        return;
    }
    KnobChangeTimeout--;

    for(int i = 0; i < NEOPIXEL_COUNT; i++)
    {
        if((float)i/(float)NEOPIXEL_COUNT < threshold)
        {
            uint32_t rgbcolor = neopixelStrip.ColorHSV(BLUE_HUE, 255, 50);
            neopixelStrip.setPixelColor(i, rgbcolor);
        }
        else
        {
            uint32_t rgbcolor = neopixelStrip.ColorHSV(BLUE_HUE, 255, 10);
            neopixelStrip.setPixelColor(i, rgbcolor);
        }
    }
}

void readButtonHold()
{
    if(digitalRead(PIN_IN_BUTTON_RESET) == 1)
    {
        accumulatedButtonHold++;
    }
    else
    {
        accumulatedButtonHold = 0;
    }

    if(accumulatedButtonHold > holdLength)
    {
        accumulatedButtonHold  = -holdLength;
        isInitialized = false;
    }

    if(accumulatedButtonHold > 1)
    {
        for(int i = 0; i < NEOPIXEL_COUNT; i++)
        {
            if(i%8 <= accumulatedButtonHold/8)
            {
                uint32_t rgbcolor = neopixelStrip.ColorHSV(RED_HUE, 255, 50);
                neopixelStrip.setPixelColor(i, rgbcolor);
            }
            else
            {
                uint32_t rgbcolor = neopixelStrip.ColorHSV(RED_HUE, 255, 10);
                neopixelStrip.setPixelColor(i, rgbcolor);
            }
        }
    }
}

void ClearDisplay()
{
    for(int i = 0; i < NEOPIXEL_COUNT; i++)
    {

        uint32_t rgbcolor = neopixelStrip.ColorHSV(0, 0, 10);
        neopixelStrip.setPixelColor(i, rgbcolor);
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    ClearDisplay();

    if(!isInitialized)
    {

        //27,28,35,36

        uint32_t rgbcolor = neopixelStrip.ColorHSV(0, 0, 100);
        neopixelStrip.setPixelColor(27, rgbcolor);
        neopixelStrip.setPixelColor(28, rgbcolor);
        neopixelStrip.setPixelColor(35, rgbcolor);
        neopixelStrip.setPixelColor(36, rgbcolor);
        ShowNeopixels();

        delay(1000);


        for(int i = 0; i < READINGS_COUNT; i ++)
        {
            SetInitializationReading(i);
            delayMicroseconds(1000);
        }

        SetAmbientAverage();
        isInitialized = true;

        uint32_t rgbcolor2 = neopixelStrip.ColorHSV(0, 0, 0);
        neopixelStrip.setPixelColor(27, rgbcolor2);
        neopixelStrip.setPixelColor(28, rgbcolor2);
        neopixelStrip.setPixelColor(35, rgbcolor2);
        neopixelStrip.setPixelColor(36, rgbcolor2);

        ReadKnobAverage(true);

        ShowNeopixels();
    }
    else
    {
        EvaluateAllKeys();
        EvaluateArrows();
        ReadKnobAverage(false);
        readButtonHold();
        DisplaySensitivityThreshold();
        ShowNeopixels();
        delay(1);
        bleGamepad.sendReport();
    }

}