#ifndef __TELEOPDRIVER__
#define __TELEOPDRIVER__

#include "sharedutils.h"
#include "JoystickDriver4278.c"

//Allows threshold to be defined in teleop-file
#ifndef THRESHOLD
#define THRESHOLD 10.0
#endif

#define MINX  10.0
#define SLOPE 0.5
#define DISTA 0.6
float powscl(int xz) {
	float sign = (float)sgn(xz);
	float x = abs(xz)/128.0;
	if(x < DISTA) {return 100 * sign * (x*SLOPE);}
		else {return 100 * sign * ((DISTA*SLOPE*(x-1.0) - x + DISTA) / (DISTA - 1.0));}
}

//Controller 1 - Left Joystick - Linear
#define JOY_X1 (abs(joystick.joy1_x1) > THRESHOLD ? joystick.joy1_x1 : 0)
#define JOY_Y1 (abs(joystick.joy1_y1) > THRESHOLD ? joystick.joy1_y1 : 0)

//Defines current button map layout
#define JOY_X 0
#define JOY_Y 3
#define JOY_B 2
#define JOY_A 1

#define JOY_RB 5
#define JOY_LB 4
#define JOY_LT 6
#define JOY_RT 7

#define JOY_R3 10
#define JOY_L3 11

#define JOY_ST 9
#define JOY_BA 8

#define JOY_BTN joystick.joy1_Buttons

void waitForStart() {
  while(true) {
    getJoystickSettings(joystick);
    if(!joystick.StopPgm) break;
    displayDiagnostics();
  }
}

#endif //__TELEOPUTILS__
