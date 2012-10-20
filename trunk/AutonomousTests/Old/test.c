#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     HTAccel,        sensorI2CHiTechnicAccel)
#pragma config(Sensor, S2,     htGyro,         sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S4_C1_1,     motorBL,       tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S4_C1_2,     motorFL,       tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S4_C2_1,     motorRArm,     tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S4_C2_2,     motorLArm,     tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_1,     motorBR,       tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S4_C3_2,     motorFR,       tmotorNormal, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "HTGYRO-driver.h"
#include "drivers/HTAC-driver.h"
#include "common.h"

#define mBackLeft motor[motorBL]
#define mBackRight motor[motorBR]
#define mFrontLeft motor[motorFL]
#define mFrontRight motor[motorFR]
#define mRArm motor[motorRArm]
#define mLArm motor[motorLArm]

#define joyX joystick.joy1_x1
#define joyY joystick.joy1_y1
#define joyRot joystick.joy1_x2
#define threshX (abs(joyX) > 6 ? true:false)
#define threshY (abs(joyY) > 6 ? true:false)
#define threshRot (abs(joyRot) > 6 ? true:false)

float robotAngle = 0;
float lastTime = 0;
//0.75 1.00 1.25
void calculateGyro()
{
  if(abs(HTGYROreadRot(S2)) > 1.0)
  {
    robotAngle += (nPgmTime - lastTime) * HTGYROreadRot(S2) / (1000);
  }
  lastTime = nPgmTime;
}

int xAxis, yAxis, zAxis, xThresh, yThresh, zThresh;
float xAcc, yAcc, zAcc, xVel, yVel, zVel, xDist, yDist, zDist;

void calculateAcc() {
  HTACreadAllAxes(HTAccel, xAxis, yAxis, zAxis); xAxis = xAxis - xThresh; yAxis = yAxis - yThresh; zAxis = zAxis - zThresh;
    xAcc = xAxis/20.0; yAcc = yAxis/20.0; zAcc = zAxis/20.0;

    nxtDisplayTextLine(0, "xA: "+xAcc);
    nxtDisplayTextLine(1, "yA: "+yAcc);
    nxtDisplayTextLine(2, "zA: "+zAcc);

    if(abs(xAcc) > .1) xVel += (((float)nPgmTime - lastTime)/1000.0) * xAcc;
    if(abs(yAcc) > .1) yVel += (((float)nPgmTime - lastTime)/1000.0) * yAcc;
    if(abs(zAcc) > .1) zVel += (((float)nPgmTime - lastTime)/1000.0) * zAcc;

    nxtDisplayTextLine(3, "xV: "+xVel);
    nxtDisplayTextLine(4, "yV: "+yVel);
    nxtDisplayTextLine(5, "zV: "+zVel);

}

float getTheta(int x, int y)
{
  return atan2(x, y);
}

float getMagnitude(int x, int y)
{
  return sqrt(pow(x,2)+pow(y,2)) / 181.019336;
}

float getRotation(int x)
{
  return ((float)x)/128;
}

void stopMotors()
{
  mBackLeft = 0;
  mBackRight = 0;
  mFrontLeft = 0;
  mFrontRight = 0;
}

void computeMovement()
{
  getJoystickSettings(joystick);
  if(!threshX && !threshY && !threshRot) {stopMotors(); return;}

  float th = getTheta(joyX, joyY);
  float v = getMagnitude(joyX, joyY);
  float rot = getRotation(joyRot);
  float gyr = degreesToRadians(robotAngle);

  float mBackLeftTmp = (cos(th+(PI/4)-gyr)*v - rot/(1.5));
  float mFrontRightTmp = (cos(th+(PI/4)-gyr)*v + rot/1.5);
  float mBackRightTmp = -1 * (sin(th+(PI/4)-gyr)*v - rot/1.5);
  float mFrontLeftTmp = -1 * (sin(th+(PI/4)-gyr)*v + rot/1.5);

  float max = 1;
  if(abs(mBackLeftTmp) > abs(max)) max = mBackLeftTmp;
  if(abs(mFrontRightTmp) > abs(max)) max = mFrontRightTmp;
  if(abs(mBackRightTmp) > abs(max)) max = mBackRightTmp;
  if(abs(mFrontLeftTmp) > abs(max)) max = mFrontLeftTmp;
  max = abs(max);

  mBackLeft = 100 * mBackLeftTmp / max;
  mFrontRight = 100 * mFrontRightTmp / max;
  mBackRight = 100 * mBackRightTmp / max;
  mFrontLeft = 100 * mFrontLeftTmp / max;
}

void checkButtons()
{
  if(joy1Btn(2)) {HTGYROstartCal(S2); PlaySound(soundBeepBeep);}
  if(joy1Btn(5)) {mRArm =  -95; mLArm = -95;}
  if(joy1Btn(6)) {mRArm = 95; mLArm =  95;}
  if(!joy1Btn(5) && !joy1Btn(6)) {mRArm = 0; mLArm = 0;}
}

task main()
{
  HTGYROstartCal(S2);
  lastTime = nPgmTime;
  HTACreadAllAxes(HTAccel, xThresh, yThresh, zThresh);
  xAcc = 0; yAcc = 0; zAcc = 0; xVel = 0; yVel = 0; zVel = 0; xDist = 0; yDist = 0; zDist = 0;
  while(true)
  {
    computeMovement();
    //nxtDisplayTextLine(5, ""+(nPgmTime - lastTime));
    calculateGyro();
    calculateAcc();
    checkButtons();
    //nxtDisplayTextLine(3, ""+HTGYROreadRot(S2));
    //nxtDisplayTextLine(4, ""+robotAngle);
    wait1Msec(2);
  }
}
