#pragma once

#define OBJECT_SCALE 2.0
#define M_PI 3.1415926
// for perspective
#define OBJECT_DISTANCE 400.f
#define PERSPECTIVE_FOV 30.f
#define VIEW_NEAR 0.1
#define VIEW_FAR 600.f

// for orthogonal
// = distance*tan(fov/2)
#define VIEW_BOTTOM -OBJECT_DISTANCE*tanf(PERSPECTIVE_FOV/360.f*M_PI)
#define VIEW_TOP OBJECT_DISTANCE*tanf(PERSPECTIVE_FOV/360.f*M_PI)

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


