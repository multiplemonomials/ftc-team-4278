#ifndef __SHAREDUTILS__
#define __SHAREDUTILS__

#include "JoystickDriver4278.c"
#include "hitechnic-irseeker-v2.h"
#include "wiringnxt.h"
#include "Feedback.h"

#define setLeftMotors(x)  {int pow = multiplierLeft*x; motor[mLeft1] = pow; motor[mLeft2] = pow;}
#define setRightMotors(x) {int powr = multiplierRight*x; motor[mRight1] = powr; motor[mRight2] =  powr;}

#define setArmLocked()   {servo[sLock] = 140;}
#define setArmUnlocked() {servo[sLock] = 080;}

#define setShifterArm()  {servo[sShifter] = 100; shifterStateArm = true;}
#define setShifterFlag() {servo[sShifter] = 170; shifterStateArm = false;}
#define getShifterState() (shifterStateArm)
#define flipShifterState() {if(shifterStateArm) setShifterFlag(); else setShifterArm();}

#define leftEncoder     abs(nMotorEncoder[mLeft2])
#define rightEncoder    abs(nMotorEncoder[mRight1])

#define clearEncoders() {nMotorEncoder[mLeft2] = 0; nMotorEncoder[mRight1] = 0; encodersWereCleared = true;}

//Distance Macros
#define INCH   1.0
#define CM     0.3937
#define MM    39.370
#define YARD  36.0
#define FOOT  12.0
#define METER 39.370

#define WHEELCIRC 9.42585

//horizontal distance between wheels
#define WHEELBASE 10.95
#define FLOORMAT  24.0

#define BTN_CENTER 3
#define BTN_LEFT   2
#define BTN_RIGHT  1
#define BTN_BACK   0

#define LEFT_POW_DIFF 1.0
#define RIGHT_POW_DIFF 1.0

//degrees per revolution
#define DEG_PER_REV 1440

void waitForStart() {
  while(true) {
    getJoystickSettings(joystick);
    if(!joystick.StopPgm) break;
  }
}

void extendArm()
{
	//58.5 is the maximum torque speed
	motor[mArm1] = 58.5;
	motor[mArm2] = 58.5;
}

void stopArm()
{
	//58.5 is the maximum torque speed
	motor[mArm1] = 0;
	motor[mArm2] = 0;
}

void retractHooks()
{
	servo[tubeHook1] = 239;
	servo[tubeHook2] = 16;
}

float max(float a, float b) {return (a>b ? a : b);}

#endif //__SHAREDUTILS__
