/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#define intakeInBtn (joystickGetDigital(1,5,2))
#define intakeOutBtn (joystickGetDigital(1,5,1))
#define driveLeftStk (joystickGetAnalog(1,3))
#define driveRightStk (joystickGetAnalog(1,2))
const unsigned int motors[128] =
{
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0, 21, 21, 21, 22, 22, 22, 23, 24, 24,
 25, 25, 25, 25, 26, 27, 27, 28, 28, 28,
 28, 29, 30, 30, 30, 31, 31, 32, 32, 32,
 33, 33, 34, 34, 35, 35, 35, 36, 36, 37,
 37, 37, 37, 38, 38, 39, 39, 39, 40, 40,
 41, 41, 42, 42, 43, 44, 44, 45, 45, 46,
 46, 47, 47, 48, 48, 49, 50, 50, 51, 52,
 52, 53, 54, 55, 56, 57, 57, 58, 59, 60,
 61, 62, 63, 64, 65, 66, 67, 67, 68, 70,
 71, 72, 72, 73, 74, 76, 77, 78, 79, 79,
 80, 81, 83, 84, 84, 86, 86, 87, 87, 88,
 88, 89, 89, 90, 90,127,127,127
};

// Local Function Prototypes
void autoStack();
bool valid();
void manualIntake();
void manualLift();

int autoState = 0;
int stackHeight = baseStackHeight;

void operatorControl() {

  bool autoStackEnabled = true;
  TaskHandle autoTaskHandle = null;

	while (1) {

    bool autoStackOnBtn = joystickGetDigital(1, 8, JOY_DOWN);
    bool autoStackOffBtn = joystickGetDigital(1, 8 JOY_UP);
    if(autoStackOnBtn) autoStackEnabled = true;
    else if(autoStackOffBtn) autoStackEnabled = false;
    if(autoState == -1){
      taskDelete(autoStackHandle);
      autoStatus = 0;
    }
    // Auto Stacking
    if(autoStackEnabled){
      if(autoState == 0 && intakeInBtn){
        autoStackHandle = taskCreate(autoStack, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
      }
    }
    else{
      autoState = 0;
      manualIntake();
      manualLift();
    }

    bool resetStackHeightBtn = joystickGetDigital(1,8,JOY_RIGHT);
    if(resetStackHeightBtn) stackHeight = baseStackHeight;

		// Mogo
		int mogoOutBtn = joystickGetDigital(1, 7, 2);
		int mogoInBtn = joystickGetDigital(1, 7, 1);
		mogoSet((mogoOutBtn - mogoInBtn) * 127);

		// Drive
		driveSet(driveLeftStk, driveRightStk);

		delay(20);
	}
}

void autoStack(){
  // Change autostate to executing
  autoState = 1;
  // Wait for cone in intake
  while(intakeLim == 1 && valid()){
    manualIntake();
    delay(40);
  }
  // Wait for lift button
  while(!liftUpBtn && valid()) delay(100);
  // Lift to height
  liftTo(stackHeight);
  stackHeight+=coneIncrement;
  // Intake to stack
  intakeSet(127);
  delay(500);
  intakeSet(0);
  // Lift to bottom
  liftTo(liftBottom);
  // Reset autostate
  autoState = 0;
}

bool valid(){
  bool btn = joystickGetDigital(1,6,1);
  if(btn) autoStatus = -1;
  return !btn;
}

void manualIntake(){
  int inBtn = intakeInBtn;
  int outBtn = intakeOutBtn;
  intakeSet((inBtn - outBtn) * 127);
}

// void autoLift(){
//   int height = (analogRead(potL) + analogRead(potR)) / 2;
//   int cancleBtn = joystickGetAnalog(1, 6, JOY_DOWN);
//   // waiting for lift btn
//   if(autoState==1){
//     if(joystickGetDigital(1, 6, JOY_UP)) autoState=2;
//   }
//
//   // lifting
//   else if(autoState == 2){
//     if(height < stackHeight){
//       liftSet(127);
//     }
//     else{
//       liftSet(0);
//       stackHeight += coneIncrement;
//       autoState = 3;
//     }
//   }
//
//   // stacking
//   else if(autoState == 3){
//     intakeSet(127);
//     static int cycleCounter = 0;
//     if(cycleCounter < 50){
//       cycleCounter++;
//     }
//
//   }
// }

void manualLift(){
  int liftUpBtn = joystickGetDigital(1, 6, 2);
  int liftDownBtn = joystickGetDigital(1, 6, 1);
  liftSet((liftUpBtn - liftDownBtn) * 127);
}

void liftTo(int height){
  int liftHeight = (analogRead(liftL) + analogRead(liftR))/2;
  if(liftHeight < height){
    liftSet(127);
    while(liftHeight < height && valid()){
      liftHeight = (analogRead(liftL) + analogRead(liftR))/2;
      delay(20);
    }
  }
  else{
    liftSet(-127);
    while(liftHeight > height && valid()){
      liftHeight = (analogRead(liftL) + analogRead(liftR))/2;
      delay(20);
    }
  }
  liftSet(0);
}

// Global Functions
void driveSet(int left, int right){
	left = motors[left];
	right = motors[right];
	motorSet(driveOutL, -left);
	motorSet(driveMidL, left);
	motorSet(driveOutR, -right);
	motorSet(driveMidR, right);
}

void liftSet(int power){
	motorSet(liftL, power);
	motorSet(liftR, -power);
}

void intakeSet(int power){
	motorSet(intakeL, power);
	motorSet(intakeR, -power);
}

void mogoSet(int power){
	motorSet(mogoL, -power);
	motorSet(mogoR, power);
}
