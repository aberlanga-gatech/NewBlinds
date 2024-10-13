#include <HomeSpan.h>
#include <TMCStepper.h>
/**
 * Implementation of the TMC2209 Stepper Driver IC using the TMCStepper library for compatibility with HomeSpan
 * Creates a TMC2209_Stepper object
 * Able to natively support:
 * - UART communication
 * - StallGuard
 * - Microstepping, current control, etc.
 *
 */

//////////////////////////

struct Stepper_TMC : StepperControl
{
    TMC2209Stepper driver;
    int stepPin;
    int dirPin;
    int enablePin;

    //////////////////////////

    Stepper_TMC(Stream &serial, float r_sense, uint8_t driver_address, int stepPin, int dirPin, int enablePin, std::pair<uint32_t, uint32_t> taskParams = {1, 0}) : StepperControl(taskParams.first, taskParams.second), driver(&serial, r_sense, driver_address)
    {
        this->stepPin = stepPin;
        this->dirPin = dirPin;
        this->enablePin = enablePin;

        pinMode(stepPin, OUTPUT);
        pinMode(dirPin, OUTPUT);
        pinMode(enablePin, OUTPUT);

        digitalWrite(enablePin, HIGH); // Disable driver by default

        driver.begin();               // Initialize driver
        driver.toff(5);               // Enable driver
        driver.rms_current(600);      // Set motor current to 600mA
        driver.microsteps(8);         // Set microsteps to 8
        driver.pdn_disable(true);     // Use UART communication
        driver.I_scale_analog(false); // Set current scaling mode
    }

    //////////////////////////

    void onStep(boolean direction) override
    {
        digitalWrite(dirPin, direction);
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1);
        digitalWrite(stepPin, LOW);
    }

    //////////////////////////

    void onEnable() override
    {
        digitalWrite(enablePin, LOW); // Enable driver
    }

    //////////////////////////

    void onDisable() override
    {
        digitalWrite(enablePin, HIGH); // Disable driver
    }

    //////////////////////////

    StepperControl *setStepType(int mode) override
    {
        switch (mode)
        {
        case FULL_STEP_TWO_PHASE:
            driver.microsteps(1);
            break;
        case HALF_STEP:
            driver.microsteps(2);
            break;
        case QUARTER_STEP:
            driver.microsteps(4);
            break;
        case EIGHTH_STEP:
            driver.microsteps(8);
            break;
        default:
            ESP_LOGE(STEPPER_TAG, "Unknown StepType=%d", mode);
        }
        return (this);
    }
};