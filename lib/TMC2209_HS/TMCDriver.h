#include "HomeSpan.h"
#include <TMCStepper.h>
/**
 * Implementation of the TMC2209 STepper Driver IC using the TMCDriver library for compatibility with HomeSpan
 * Creates a TMC2209_Steper object
 * Able to natively support:
 * - UART communication
 * - StallGuard (coming soon)
 * - etc...
 *
 */

//////////////////////////

struct Stepper_TMC : StepperControl
{
    int m1Pin;
    int m2Pin;
    int stepPin;
    int dirPin;
    int enablePin;

    //////////////////////////

    Stepper_TMC(int m1Pin, int m2Pin, int stepPin, int dirPin, int enablePin, std::pair<uint32_t, uint32_t> taskParams = {1, 0}) : StepperControl(taskParams.first, taskParams.second)
    {
        this->m1Pin = m1Pin;
        this->m2Pin = m2Pin;
        this->stepPin = stepPin;
        this->dirPin = dirPin;
        this->enablePin = enablePin;

        pinMode(m1Pin, OUTPUT);
        pinMode(m2Pin, OUTPUT);
        pinMode(stepPin, OUTPUT);
        pinMode(dirPin, OUTPUT);
        pinMode(enablePin, OUTPUT);

        setStepType(FULL_STEP_TWO_PHASE);
    }

    //////////////////////////

    void onStep(boolean direction) override
    {
        digitalWrite(dirPin, direction);
        digitalWrite(stepPin, HIGH);
        digitalWrite(stepPin, LOW);
    }

    //////////////////////////

    void onEnable() override
    {
        digitalWrite(enablePin, LOW);
    }

    //////////////////////////

    void onDisable() override
    {
        digitalWrite(enablePin, HIGH);
    }

    //////////////////////////

    StepperControl *setStepType(int mode) override
    {
        switch (mode)
        {
        case FULL_STEP_TWO_PHASE:
            digitalWrite(m1Pin, LOW);
            digitalWrite(m2Pin, LOW);
            break;
        case HALF_STEP:
            digitalWrite(m1Pin, HIGH);
            digitalWrite(m2Pin, LOW);
            break;
        case QUARTER_STEP:
            digitalWrite(m1Pin, LOW);
            digitalWrite(m2Pin, HIGH);
            break;
        case EIGHTH_STEP:
            digitalWrite(m1Pin, HIGH);
            digitalWrite(m2Pin, HIGH);
            break;
        default:
            ESP_LOGE(STEPPER_TAG, "Unknown StepType=%d", mode);
        }
        return (this);
    }
};