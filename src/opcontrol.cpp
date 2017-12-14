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

void operatorControl() {
	while (1) {



		// Mogo
		int mogoOutBtn = joystickGetDigital(1, 7, 2);
		int mogoInBtn = joystickGetDigital(1, 7, 1);
		mogoSet((mogoOutBtn - mogoInBtn) * 127);

		// Intake
		int intakeInBtn = joystickGetDigital(1, 5, 2);
		int intakeOutBtn = joystickGetDigital(1, 5, 1);
		intakeSet((intakeInBtn - intakeOutBtn) * 127);

		// Lift
		int liftUpBtn = joystickGetDigital(1, 6, 2);
		int liftDownBtn = joystickGetDigital(1, 6, 1);
		liftSet((liftUpBtn - liftDownBtn) * 127);

		// Drive
		int driveLeftStk = joystickGetAnalog(1, 3);
		int driveRightStk = joystickGetAnalog(1, 2);
		driveSet(driveLeftStk, driveRightStk);

		delay(20);
	}
}

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
