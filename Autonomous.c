#pragma config(Hubs,   S1,  HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,   S2,  HTServo,  none,     none,     none)
#pragma config(Hubs,   S4,  HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,         ,  sensorI2CMuxController)
#pragma config(Sensor, S2,         ,  sensorI2CMuxController)
#pragma config(Sensor, S3, sensorIR,  sensorI2CCustom)
#pragma config(Sensor, S4,    HTSPB,  sensorI2CMuxController)
#pragma config(Motor,   mtr_S1_C1_1,   mArm2, tmotorTetrix, openLoop, encoder)
#pragma config(Motor,   mtr_S1_C1_2,   mArm1, tmotorTetrix, openLoop, encoder)
#pragma config(Motor,   mtr_S1_C2_1, mRight1, tmotorTetrix, openLoop, reversed)
#pragma config(Motor,   mtr_S1_C2_2, mRight2, tmotorTetrix, openLoop, reversed)
#pragma config(Motor,   mtr_S4_C1_1,  mLeft1, tmotorTetrix, openLoop)
#pragma config(Motor,   mtr_S4_C1_2,  mLeft2, tmotorTetrix, openLoop)
#pragma config(Motor,   mtr_S4_C2_1,   mSpin, tmotorTetrix, openLoop)
#pragma config(Motor,   mtr_S4_C2_2,  mNull1, tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1, servoL2, tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2, servoL1, tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,  servo3, tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,  servo4, tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,  servo5, tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,  servo6, tServoNone)
//*!!Codez automagically venerated by 'ROWBOT SEA' conflagration lizard               !!*//

#define POST_BRIDGE true
#define __CALIBRATED__

#include "drivers/autoutils.h"
#include "autoconst.h"

void initializeRobot() {unlockArmMotors();}
void moveBridge() {
	setLeftMotors(60);
	setRightMotors(60);
	wait1Msec(3200);
	setLeftMotors(0);
	setRightMotors(0);
}

void runAutoLeft() {
	int dirIR, strIR; float stopRightEnc;
	HTIRS2readEnhanced(sensorIR, dirIR, strIR);
	clearEncoders();

	while(dirIR != 5 && rightEncoder < 4325) {
		HTIRS2readEnhanced(sensorIR, dirIR, strIR);
		stopRightEnc = rightEncoder;
		if(dirIR != 5) {setLeftMotors(60); setRightMotors(60);}
	}
	setLeftMotors(0); setRightMotors(0); pause(3);

}

int OPT_SIDE = 0; int OPT_AUTO = 0; int OPT_DELAY = 0; int OPT_BRIDGE = 0;
void optionScreen() {
	nxtDisplayTextLine(0, "NXT:  %.2f V", ((float)nAvgBatteryLevel)/1000.0);
	if(externalBatteryAvg > 0) nxtDisplayTextLine(1, "EXT: %.2f V", ((float)externalBatteryAvg)/1000.0);
		else nxtDisplayTextLine(1, "EXT: OFF");

	if(nAvgBatteryLevel < NXT_LOW_BAT) nxtDisplayTextLine(2, "***NXT     LOW***");
	if(externalBatteryAvg < EXT_LOW_BAT) nxtDisplayTextLine(2, "***    EXT LOW***");
	if(nAvgBatteryLevel < NXT_LOW_BAT && externalBatteryAvg < EXT_LOW_BAT) nxtDisplayTextLine(2, "***NXT EXT LOW***");

	while(nNxtButtonPressed != BTN_CENTER) { // SIDE: LEFT | RIGHT | BRIDGE | NONE
		if(OPT_SIDE == 0) nxtDisplayTextLine(3, "SIDE: Left");
		if(OPT_SIDE == 1) nxtDisplayTextLine(3, "SIDE: Right");
		if(OPT_SIDE == 2) nxtDisplayTextLine(3, "SIDE: Bridge");
		if(OPT_SIDE == 3) nxtDisplayTextLine(3, "SIDE: None");
		if(nNxtButtonPressed == BTN_LEFT || nNxtButtonPressed == BTN_RIGHT) {
			PlaySound(soundShortBlip);
			if(nNxtButtonPressed == BTN_LEFT) OPT_SIDE--;
			if(nNxtButtonPressed == BTN_RIGHT) OPT_SIDE ++;
			if(OPT_SIDE > 4) OPT_SIDE = 0;
			if(OPT_SIDE < 0) OPT_SIDE = 4;

			while(nNxtButtonPressed == BTN_LEFT || nNxtButtonPressed == BTN_RIGHT) wait1Msec(5);
		}
	} PlaySound(soundShortBlip); while(nNxtButtonPressed == BTN_CENTER) wait1Msec(5);

	if(OPT_SIDE != 3) // DELAY: 0 - 25000
		while(nNxtButtonPressed != BTN_CENTER) {
			nxtDisplayTextLine(4, "DELY: %i", OPT_DELAY);
			if(nNxtButtonPressed == 1 || nNxtButtonPressed == 2) {
				PlaySound(soundShortBlip);
				if(nNxtButtonPressed == 2) OPT_DELAY -= 1000;
				if(nNxtButtonPressed == 1) OPT_DELAY += 1000;
				if(OPT_DELAY < 0)     OPT_DELAY = 25000;
				if(OPT_DELAY > 25000) OPT_DELAY = 0;

				while(nNxtButtonPressed == BTN_LEFT || nNxtButtonPressed == BTN_RIGHT) wait1Msec(5);
			}
		} PlaySound(soundShortBlip); while(nNxtButtonPressed == BTN_CENTER) wait1Msec(5);

	if(OPT_SIDE < 2) // AUTO: IR | Crate 1 | Crate 2 | Crate 3 | Crate 4
		while(nNxtButtonPressed != BTN_CENTER) {
			if(OPT_AUTO == 0) nxtDisplayTextLine(5, "AUTO: IR");
			else nxtDisplayTextLine(5, "AUTO: Crate %i", OPT_AUTO);

			if(nNxtButtonPressed == BTN_LEFT || nNxtButtonPressed == BTN_RIGHT) {
				PlaySound(soundShortBlip);
				if(nNxtButtonPressed == BTN_LEFT) OPT_AUTO--;
				if(nNxtButtonPressed == BTN_RIGHT) OPT_AUTO++;
				if(OPT_AUTO > 4) OPT_AUTO = 0;
				if(OPT_AUTO < 0) OPT_AUTO = 4;

				while(nNxtButtonPressed == BTN_LEFT || nNxtButtonPressed == BTN_RIGHT) wait1Msec(5);
			}
		} PlaySound(soundShortBlip); while(nNxtButtonPressed == BTN_CENTER) wait1Msec(5);

	if(OPT_SIDE < 2) // BRIDGE: Closest | Left | Right | Back up | None
		while(nNxtButtonPressed != BTN_CENTER) {
			     if(OPT_BRIDGE == 0) nxtDisplayTextLine(6, "BRDG: Closest");
			else if(OPT_BRIDGE == 1) nxtDisplayTextLine(6, "BRDG: Left");
			else if(OPT_BRIDGE == 2) nxtDisplayTextLine(6, "BRDG: Right");
			else if(OPT_BRIDGE == 3) nxtDisplayTextLine(6, "BRDG: Back up");
			else if(OPT_BRIDGE == 4) nxtDisplayTextLine(6, "BRDG: None");

			if(nNxtButtonPressed == BTN_LEFT || nNxtButtonPressed == BTN_RIGHT) {
				PlaySound(soundShortBlip);
				if(nNxtButtonPressed == BTN_LEFT) OPT_BRIDGE--;
				if(nNxtButtonPressed == BTN_RIGHT) OPT_BRIDGE++;
				if(OPT_BRIDGE > 4) OPT_BRIDGE = 0;
				if(OPT_BRIDGE < 0) OPT_BRIDGE = 4;

				while(nNxtButtonPressed == BTN_LEFT || nNxtButtonPressed == BTN_RIGHT) wait1Msec(5);
			}
		} PlaySound(soundShortBlip); while(nNxtButtonPressed == BTN_CENTER) wait1Msec(5);

	nxtDisplayTextLine(7, "*** LOCKED ***");
}

task main() {
	initializeRobot();
	optionScreen();
	waitForStart();
	wait1Msec(OPT_DELAY);

	if(OPT_SIDE == 0) { //Left
	} else if(OPT_SIDE == 1) { //Right
	} else if(OPT_SIDE == 2) moveBridge(); //Bridge

	lockdownRobot();
}
