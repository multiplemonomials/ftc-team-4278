#define gyrThresh 0.15
#define aToN 20.0
#define aThresh 0.1
#define vThresh 0.1
#define posToFeet 12

#include "drivers\hitechnic-accelerometer.h"
#include "drivers\hitechnic-gyro.h"

float robotTh=0;

task GyroIntegrate()
{
	HTGYROstartCal(sGyr);
	int lastIterTime=0;
	while(true) {
		if(abs(HTGYROreadRot(sGyr)) > gyrThresh) robotTh += (9.0/2.0) * HTGYROreadRot(sGyr) * (float)(nPgmTime - lastIterTime) / 1000.0;
		nxtDisplayTextLine(0, "Rot: %f", robotTh);
		lastIterTime = nPgmTime;
		EndTimeSlice();
	}
}

float robotX, robotY;

task AccelIntegrate()
{
  int xAxis=0, yAxis=0, zAxis=0, xBias=0, yBias=0, zBias=0;
  float xAcc=0, yAcc=0, xVel=0, yVel=0, xPos=0, yPos=0, xScl = 0.973236, yScl = 0.977995;
  float xAccAvg[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, yAccAvg[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  HTACreadAllAxes(sAcc, xBias, yBias, zBias);
  float lastTime;
  lastTime = nPgmTime;

  while(true)
  {
    HTACreadAllAxes(sAcc, xAxis, yAxis, zAxis);
    xAcc = (xAxis - xBias)*xScl/aToN; yAcc = (yAxis-yBias)*yScl/aToN;

    float xAccSum=0, yAccSum=0;
    for(int i = 0; i < 14; i++) {
    	xAccAvg[i] = xAccAvg[i+1];
    	yAccAvg[i] = yAccAvg[i+1];
    	xAccSum += xAccAvg[i];
    	yAccSum += yAccAvg[i];
  	}

    xAccAvg[14] = xAcc; xAccSum += xAcc; xAccSum /= 15;
    yAccAvg[14] = yAcc; yAccSum += yAcc; yAccSum /= 15;
   	float dT = (nPgmTime - lastTime)/1000.0;

   	xVel += xAccSum*dT*(abs(xAccSum) > aThresh ? 1.0 : 0.0); yVel += yAccSum*dT*(abs(yAccSum) > aThresh ? 1.0 : 0.0);
   	xPos += xVel*dT*posToFeet*(abs(xVel) > vThresh ? 1.0 : 0.0); yPos += yVel*dT*posToFeet*(abs(yVel) > vThresh ? 1.0:0.0);

    nxtDisplayTextLine(1, "xA: %f",xAccSum);
    nxtDisplayTextLine(2, "yA: %f",yAccSum);
    nxtDisplayTextLine(3, "xV: %f",xVel);
    nxtDisplayTextLine(4, "yV: %f",yVel);
    nxtDisplayTextLine(5, "xP: %f",xPos);
    nxtDisplayTextLine(6, "yP: %f",yPos);
    nxtDisplayTextLine(7, "t:  %f",dT);
    lastTime = nPgmTime;

    robotX = xPos; robotY = yPos;
  }
}

void BackgroundIntegration()
{
	StartTask(GyroIntegrate);
	StartTask(AccelIntegrate);
}
