#pragma once


#define OBJECT_DISTANCE 200
#define PERSPECTIVE_FOV 30
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_ASPECT (WINDOW_WIDTH/(float)WINDOW_HEIGHT)
#define VIEW_NEAR 50
#define VIEW_FAR 400
#define VIEW_LEFT -100
#define VIEW_RIGHT 100
#define VIEW_BOTTOM (VIEW_LEFT/WINDOW_ASPECT)
#define VIEW_TOP (VIEW_RIGHT/WINDOW_ASPECT)

#define SCALE_MIN 0.06
#define SCALE_MAX 0.57

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


