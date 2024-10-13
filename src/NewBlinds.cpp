#include <Arduino.h>
#include <HomeSpan.h>
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

////////////////////////////////////
// Create Stepper_TMC Instance
Stepper_TMC stepperDriver(SERIAL_PORT, R_SENSE, DRIVER_ADDRESS, STEP_PIN, DIR_PIN, EN_PIN);

////////////////////////////////////
// Define WindowCovering Accessory Class
struct DEV_WindowShade : Service::WindowCovering
{

  Characteristic::CurrentPosition currentPos{0, true}; // 0 = fully closed
  Characteristic::TargetPosition targetPos{0, true};   // 0 = fully closed, 100 = fully open
  Characteristic::PositionState positionState{0};      // 0 = Stopped, 1 = Increasing, 2 = Decreasing

  Stepper_TMC *shadeMotor; // Pointer to the Stepper_TMC motor
  int currentSteps;        // Tracks the current step count
  int targetSteps;         // Target number of steps to reach the desired position
  int dir;
  int delta;
  int stepsPerPercent = TOTAL_STEPS / 100;
  bool moving = false;

  DEV_WindowShade(Stepper_TMC *shadeMotor) : Service::WindowCovering()
  {
    this->shadeMotor = shadeMotor;
    this->currentSteps = 0;

    Serial.print("Initializing Blind Position (Debugging): ");
    Serial.println(currentPos.getVal());
  }

  boolean update() override
  {
    if (!moving)
    {
      targetSteps = targetPos.getNewVal() * stepsPerPercent;
      delta = targetSteps - currentSteps;

      if (delta > 0)
      {
        dir = 1; // Move Up
        currentSteps += delta;
        positionState.setVal(1); // Moving Up
      }
      else if (delta < 0)
      {
        dir = 0;                 // Move Down
        currentSteps += (delta); // Decrease step count
        positionState.setVal(2); // Moving Down
      }

      int moveSteps = abs(delta);

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
    moving = true;
    shadeMotor->onEnable();

    for (int i = 0; i < steps; i++)
    {
      shadeMotor->onStep(dir);
      delayMicroseconds(100); // Delay between steps

      // Update current position after each step
      currentPos.setVal(constrain(currentPos.getVal() + ((dir == 1 ? 1 : -1) * 1.0 / TOTAL_STEPS * 100), 0, 100));
    }

    moving = false;
    shadeMotor->onDisable();
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

  // Create a new accessory with the Window Covering service using Stepper_TMC
  configureHomeKit();
  new DEV_WindowShade(&stepperDriver);
}

////////////////////////////////////
// Main Loop
void loop()
{
  homeSpan.poll();
}
