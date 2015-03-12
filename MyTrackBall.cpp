
/*
 *  Simple trackball-like motion adapted (ripped off) from projtex.c
 *  (written by David Yu and David Blythe).  See the SIGGRAPH '96
 *  Advanced OpenGL course notes.
 */


/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
//#include <GL/glut.h>

#include "MyTrackBall.h"


MyTrackBall::MyTrackBall(void)
{
	mButtonX = 0;
	mButtonY = 0;
	mScale = 1.f;
	mAngle = 0.0; 
	mTracking = true;
	/* put the identity in the trackball transform */
	mTranslateMatrix = MyMatrixf::IdentityMatrix();
	mRotateMatrix = MyMatrixf::IdentityMatrix();
	mScaleMatrix = MyMatrixf::IdentityMatrix();
}


MyTrackBall::~MyTrackBall(void)
{
}

MyMatrixf MyTrackBall::Matrix() const
{
	return mTranslateMatrix*mRotateMatrix*mScaleMatrix;
}


void MyTrackBall::Reshape(int width, int height)
{
	mWidth  = width;
	mHeight = height;
}

void MyTrackBall::StartMotion(int x, int y){
	mButtonX = (float)x;
	mButtonY = (float)y;
	pointToVector(x, y, mWidth, mHeight, mLastPos);
}

void MyTrackBall::EndMotion(int x, int y){
	mAngle = 0.0;
}

void MyTrackBall::RotateMotion(int x, int y)
{
  MyVec3f current_position;

  if (mTracking == false)
    return;

  pointToVector(x, y, mWidth, mHeight, current_position);

  /* calculate the angle to rotate by (directly proportional to the
     length of the mouse movement */
  MyVec3f diff = current_position - mLastPos;
  mAngle = 3 * 90.0f*diff.norm();

  /* calculate the axis of rotation (cross product) */
  mAxis = mLastPos^current_position;

  /* reset for next time */
  mLastPos = current_position;

  mRotateMatrix = MyMatrixf::RotateMatrix(mAngle, mAxis[0], mAxis[1], mAxis[2])*mRotateMatrix;
}
void MyTrackBall::TranslateMotion(int x, int y, float ratio){
	translate((x - mButtonX)*ratio, (y - mButtonY)*ratio);
}

void MyTrackBall::TranslateTo(float x, float y, float z){
	mTranslateMatrix.At(0, 3) = x;
	mTranslateMatrix.At(1, 3) = y;
	mTranslateMatrix.At(2, 3) = z;
}

void MyTrackBall::TranslateMotionX(int x, int y, float ratio){
	int dx = x - mButtonX;
	int dy = y - mButtonY;
	mButtonX = x;
	mButtonY = y;
	//mTranslateMatrix = MyMatrixf::TranslateMatrix(dx / 2, -dy / 2, 0)*mTranslateMatrix;
	mTranslateMatrix = MyMatrixf::TranslateMatrix(dx / 2 *ratio, 0, 0)*mTranslateMatrix;
}


void MyTrackBall::ScaleMotion(int x, int y){
	MyVec3f current_position;
	pointToVector(x, y, mWidth, mHeight, current_position);
	float dy = current_position[1] - mLastPos[1];
	this->ScaleAdd(dy/20);
}

float MyTrackBall::GetScaleFromMotion(int x, int y) const{
	MyVec3f current_position;
	pointToVector(x, y, mWidth, mHeight, current_position);
	float dy = current_position[1] - mLastPos[1];
	return dy/20 + 1.f;
}

int MyTrackBall::GetWidth() const{
	return mWidth;
}

int MyTrackBall::GetHeight() const{
	return mHeight;
}

MyVec3f MyTrackBall::GetTotalTranslation() const{
	MyMatrixf mat = mTranslateMatrix*mRotateMatrix*mScaleMatrix;
	return MyVec3f(mat.At(0, 3), mat.At(1, 3), mat.At(2, 3));
}

void MyTrackBall::SetOrigin(const MyVec3f& origin){
	mOrigin = origin;
}

void MyTrackBall::ResetTranslate(){
	mTranslateMatrix = MyMatrixf::IdentityMatrix();
}
void MyTrackBall::ResetScale(){
	mScale = 1.f;
	mScaleMatrix = MyMatrixf::IdentityMatrix();
}
void MyTrackBall::Translate(const MyVec3f& offset){
	mTranslateMatrix = MyMatrixf::TranslateMatrix(offset[0], offset[1], offset[2])*mTranslateMatrix;
}

void MyTrackBall::translate(float dx, float dy) {
	mButtonX+=dx;
	mButtonY+=dy;
	mTranslateMatrix = MyMatrixf::TranslateMatrix(dx, -dy, 0)*mTranslateMatrix;
}

void MyTrackBall::ScaleAdd(float dy)
{
	mScale += dy;
	mScaleMatrix = MyMatrixf::ScaleMatrix(mScale, mScale, mScale);
}

void MyTrackBall::ScaleMultiply(float dy)
{
	mScale *= dy;
	mScaleMatrix = MyMatrixf::ScaleMatrix(mScale, mScale, mScale);
}

/* functions */
void MyTrackBall::pointToVector(int x, int y, int width, int height, MyVec3f& v) const
{
	float d, a;

	/* project x, y onto a hemi-sphere centered within width, height. */
	v[0] = (2.0f * x - width) / width;
	v[1] = (height - 2.0f * y) / height;
	d = sqrt(v[0] * v[0] + v[1] * v[1]);
	v[2] = cos((3.14159265f / 2.0f) * ((d < 1.0f) ? d : 1.0f));
	a = 1.0f / v.norm();
	v *= a;
}