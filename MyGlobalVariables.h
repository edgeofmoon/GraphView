#pragma once


#define OBJECT_DISTANCE 200
#define PERSPECTIVE_FOV 30
#define VIEW_NEAR 0.1
#define VIEW_FAR 400
#define VIEW_LEFT -100
#define VIEW_RIGHT 100
#define VIEW_BOTTOM -100
#define VIEW_TOP 100

#define SCALE_MIN 0.06
#define SCALE_MAX 0.57
#define LOG_BASE 2.718281828

#define FLOOR_ELEVATION -50
#define FLOOR_SIZEX 200
#define FLOOR_SIZEY 200
#define FLOOR_GRIDX 10
#define FLOOR_GRIDY 10

class MyGlobalVariables
{
public:
	MyGlobalVariables();
	~MyGlobalVariables();
};


