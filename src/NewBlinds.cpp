#include <Arduino.h>
#include <HomeSpan.h>
#include <TMCStepper.h>
#include "BlindsConfig.h"
#include "HomekitConfig.h"
#include "TMCDriver.h"

////////////////////////////////////
// Define Motor Pins and TMC2209 Settings
#define EN_PIN 2            // Enable pin
#define DIR_PIN 1           // Direction pin
#define STEP_PIN 42         // Step pin
#define SERIAL_PORT Serial1 // Use UART2 for communication
#define DRIVER_ADDRESS 0b00 // TMC2209 Address

#define R_SENSE 0.10f // Set the sense resistor value (depends on your TMC2209 setup)

// Motor Parameters
#define STEPS_PER_REV 200 // Number of steps per revolution
#define MOTOR_CURRENT 600 // Motor current in milliamps
#define MICROSTEPS 8      // Microsteps per step
#define TOTAL_STEPS (STEPS_PER_REV * TOTAL_ROTATIONS)

// Create TMC2209Stepper Instance
TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);

////////////////////////////////////
// Define WindowCovering Accessory Class
struct DEV_WindowShade : Service::WindowCovering
{

  Characteristic::CurrentPosition currentPos{0, true}; // 0 = fully closed
  Characteristic::TargetPosition targetPos{0, true};   // 0 = fully closed, 100 = fully open
  Characteristic::PositionState positionState{0};      // 0 = Stopped, 1 = Increasing, 2 = Decreasing

  TMC2209Stepper *shadeMotor; // Pointer to the TMC2209 stepper motor
  int currentSteps;           // Tracks the current step count
  int targetSteps;            // Target number of steps to reach the desired position
  int dir;
  int delta;
  int stepsPerPercent = TOTAL_STEPS / 100;
  bool moving = false;

  DEV_WindowShade(TMC2209Stepper *shadeMotor) : Service::WindowCovering()
  {
    this->shadeMotor = shadeMotor;
    this->currentSteps = 0;

    Serial.print("Initializing Blind Position: ");
    Serial.println(currentPos.getVal());

    // configure pins
    pinMode(EN_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    digitalWrite(EN_PIN, HIGH); // Disable driver initially

    // Configure TMC2209 Parameters
    shadeMotor->begin();                    // Initialize TMC2209 driver
    shadeMotor->rms_current(MOTOR_CURRENT); // Set RMS current for the motor
    shadeMotor->microsteps(MICROSTEPS);     // Set microsteps to 1/16
    shadeMotor->en_spreadCycle(false);      // Enable StealthChop for quiet mode
    shadeMotor->toff(5);                    // Enable driver

    LOG0("TMC2209 Motor initialized for Window Shade Control.\n");
  }

  /////////// Handle Position Updates from HomeKit

  boolean update() override
  {
    if (targetPos.updated())
    {
      // Calculate target position in steps
      targetSteps = targetPos.getNewVal() * stepsPerPercent;

      if (targetSteps > currentSteps)
      {
        dir = 0;
        delta = targetSteps - currentPos.getVal();
        positionState.setVal(1); // Moving Up

        // shadeMotor->shaft(1);    // Set direction to CW for increasing position
      }
      else if (targetSteps < currentSteps)
      {
        dir = 1;
        delta = currentPos.getVal() - targetSteps;
        positionState.setVal(2); // Moving Down
      }

      int moveSteps = delta * stepsPerPercent;

      Serial.print("Moving to Position: ");
      Serial.println(targetPos.getNewVal());
      if (moveSteps != 0)
      {
        moveStepper(moveSteps, dir);
      }
      positionState.setVal(0); // Stopped after movement
    }
    return true;
  }

  // Handle Stepper movement
  void moveStepper(int steps, int dir)
  {
    int stepsDelay = dir ? STEPS_DELAY_FAST : STEPS_DELAY_SLOW;
    digitalWrite(EN_PIN, LOW); // Enable driver
    moving = true;

    for (int i = 0; i < steps; i++)
    {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(stepsDelay);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(stepsDelay);

      // Update current position after each step
      currentPos.setVal(currentPos.getVal() + ((dir == 1 ? 1 : -1) * 1.0 / TOTAL_STEPS * 100));
    }

    moving = false;
    digitalWrite(EN_PIN, HIGH); // Disable driver
    LOG1("Motor Stopped at Position=%d\n", currentPos.getVal());
  }

  /////////// Monitor Motor Status and Update Position

  void loop() override
  {
    // Check if the current position has reached the target position
    if (currentSteps == targetSteps && positionState.getVal() != 0)
    {
      currentPos.setVal(targetPos.getVal()); // Update current position in HomeKit
      positionState.setVal(0);               // Stop movement
      LOG1("Motor Stopped at Position=%d\n", currentPos.getVal());
    }
  }
};

////////////////////////////////////
// HomeSpan Setup
void setup()
{
  Serial.begin(115200);
  SERIAL_PORT.begin(115200); // Start UART communication for TMC2209

  homeSpan.begin(Category::WindowCoverings, "Smart Blinds");

  // Create a new accessory with the Window Covering service using TMC2209Stepper
  configureHomeKit();
  new DEV_WindowShade(&driver);
}

////////////////////////////////////
// Main Loop
void loop()
{
  homeSpan.poll();
}
