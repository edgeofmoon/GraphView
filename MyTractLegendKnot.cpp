#include "MyTractLegendKnot.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader/Shader.h"
#include "MyUtility.h"
#include "MyGraphicsTool.h"
#include "MyPrimitiveDrawer.h"
#include "MyGlobalVariables.h"

#include <iostream>
using namespace std;

MyTractLegendKnot::MyTractLegendKnot()
{
	mFaces = 8;
	mDrawBoundingBox = false;
	mStartPoint = MyVec3f(0, -30, 0);
	mEndPoint = MyVec3f(0, 30, 0);
}


MyTractLegendKnot::~MyTractLegendKnot()
{
}

void MyTractLegendKnot::Show(){
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	//MyGraphicsTool::Sphere(10);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(mVertexArray);
	glUseProgram(mShaderProgram);

	int mvmatLocation = glGetUniformLocation(mShaderProgram, "mvMat");
	float modelViewMat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat);
	glUniformMatrix4fv(mvmatLocation, 1, GL_FALSE, modelViewMat);

	int projmatLocation = glGetUniformLocation(mShaderProgram, "projMat");
	float projMat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projMat);
	glUniformMatrix4fv(projmatLocation, 1, GL_FALSE, projMat);

	int screenSpaceLocation = glGetUniformLocation(mShaderProgram, "screenSpace");
	int screenSpace = mbScreenSpace ? 1 : 0;
	glUniform1i(screenSpaceLocation, screenSpace);

	glDrawElements(GL_TRIANGLES, mIndices.size() * 3, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
	//glDrawElements(GL_LINES, mIndices.size() * 3, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	glBindVertexArray(0);
	//glDisable(GL_CULL_FACE);

	MyGraphicsTool::Color(MyColor4f(1, 1, 1, 1));
	MyVec3f texOffset(1, -0.5, 0);
	for (int i = 0; i <= 10; i++){
		float ofr = i / (10.f);
		MyVec3f p = mStartPoint*(1 - ofr) + mEndPoint*ofr + texOffset;
		MyString value = MyString(mMinValue*(1 - ofr) + mMaxValue*ofr);
		value.resize(4);
		MyPrimitiveDrawer::DrawBitMapTextLarge(p, value, 0);
	}


	MyGraphicsTool::PopMatrix();
}
void MyTractLegendKnot::Destory(){
	glDeleteProgram(mShaderProgram);
	glDeleteVertexArrays(1, &mVertexArray);
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mNormalBuffer);
	glDeleteBuffers(1, &mTexCoordBuffer);
	glDeleteBuffers(1, &mRadiusBuffer);
	glDeleteBuffers(1, &mColorBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
}
bool MyTractLegendKnot::SeeAny(){
	return false;
}

MyGenericNode* MyTractLegendKnot::MakeSeeWhat(){
	return 0;
}

void MyTractLegendKnot::SetNumSegments(int n){
	if (n >= 3){
		mFaces = n;
	}
}

void MyTractLegendKnot::SetValueRange(const MyVec2f& valueRange){
	mMinValue = valueRange[0];
	mMaxValue = valueRange[1];
}

void MyTractLegendKnot::Build(){
	ComputeGeometry();
	LoadShader();
	LoadBuffer();
}


float MyTractLegendKnot::mapToSize(float s){
	if (mBeta > 0){
		float size = SCALE_MAX*powf(s, 1 / mBeta);
		return max(size, (float)SCALE_MIN);
	}
	else{
		float b = SCALE_MAX / (1 - LOG_BASE);
		float k = -b;
		float size = k*exp(s) + b;
		return max(size, (float)SCALE_MIN);
	}
}

void MyTractLegendKnot::ComputeGeometry(){
	int npoints = 100;
	int totalPoints = npoints * (mFaces + 1);

	mVertices.resize(totalPoints);
	mNormals.resize(totalPoints);
	mTexCoords.resize(totalPoints);
	mRadius.resize(totalPoints);
	mColors.resize(totalPoints);

	float minValue = mMinValue;
	float maxValue = mMaxValue;

	float rangeValue = maxValue - minValue;


	const float PI = 3.1415926f;
	float dangle = 2 * PI / mFaces;
	MyVec3f pole(1, 0, 0);

	for (int i = 0; i<npoints; i++){
		float offsetRatio = i / (float)npoints;
		MyVec3f p = (1 - offsetRatio)*mStartPoint + offsetRatio*mEndPoint;
		float size = offsetRatio;
		size = mapToSize(size);
		//float size = 0;
		MyVec3f d(0,1,0);

		MyVec3f perpend1 = (pole^d).normalized();
		MyVec3f perpend2 = (perpend1^d).normalized();
		//if ((perpend1^perpend2)*d < 0) dangle = -dangle;
		for (int is = 0; is<mFaces; is++){
			float angle = dangle*is;
			MyVec3f pt = sin(angle)*perpend1 + cos(angle)*perpend2;
			mVertices[i*(mFaces + 1) + is] = pt * 0 + p;
			mNormals[i*(mFaces + 1) + is] = pt;
			mTexCoords[i*(mFaces + 1) + is] = MyVec2f(i, is / (float)mFaces);
			mRadius[i*(mFaces + 1) + is] = size;
			mColors[i*(mFaces + 1) + is] = MyColor4f(0.5, 0.5, 0.5, 1);
		}
		mVertices[i*(mFaces + 1) + mFaces] = mVertices[i*(mFaces + 1)];
		mNormals[i*(mFaces + 1) + mFaces] = mNormals[i*(mFaces + 1)];
		mTexCoords[i*(mFaces + 1) + mFaces] = MyVec2f(i, 1);
		mRadius[i*(mFaces + 1) + mFaces] = mRadius[i*(mFaces + 1)];
		mColors[i*(mFaces + 1) + mFaces] = MyColor4f(0.5, 0.5, 0.5, 1);
	}


	// index
	for (int i = 1; i<npoints; i++){
		for (int j = 0; j <= mFaces; j++){
			mIndices << MyVec3i((i - 1)*(mFaces + 1) + j % (mFaces + 1),
				(i)*(mFaces + 1) + j % (mFaces + 1),
				(i)*(mFaces + 1) + (j + 1) % (mFaces + 1));
			mIndices << MyVec3i((i - 1)*(mFaces + 1) + j % (mFaces + 1),
				(i)*(mFaces + 1) + (j + 1) % (mFaces + 1),
				(i - 1)*(mFaces + 1) + (j + 1) % (mFaces + 1));
		}
	}
}

void MyTractLegendKnot::LoadBuffer(){

	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);
	// vertex
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(MyVec3f), &mVertices[0][0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(mPositionAttribute);
	glVertexAttribPointer(mPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// normal
	glGenBuffers(1, &mNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(MyVec3f), &mNormals[0][0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(mNormalAttribute);
	glVertexAttribPointer(mNormalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// texCoord
	glGenBuffers(1, &mTexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, mTexCoords.size() * sizeof(MyVec2f), &mTexCoords[0][0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(mTexCoordAttribute);
	glVertexAttribPointer(mTexCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	// radius
	glGenBuffers(1, &mRadiusBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mRadiusBuffer);
	glBufferData(GL_ARRAY_BUFFER, mRadius.size() * sizeof(float), &mRadius[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(mRadiusAttribute);
	glVertexAttribPointer(mRadiusAttribute, 1, GL_FLOAT, GL_FALSE, 0, 0);
	// color
	glGenBuffers(1, &mColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mColors.size() * sizeof(MyColor4f), &mColors[0].r, GL_STATIC_DRAW);
	glEnableVertexAttribArray(mColorAttribute);
	glVertexAttribPointer(mColorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	// texture
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	// index
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(MyVec3i), &mIndices[0][0], GL_STATIC_DRAW);
	// unbind
	glBindVertexArray(0);
}


void MyTractLegendKnot::LoadShader(){
	glDeleteProgram(mShaderProgram);
	mShaderProgram = InitShader("shader//tube_legend.vert", "shader//tube.frag", "fragColour");

	mTexUniform = glGetUniformLocation(mShaderProgram, "texUnit");
	if (mTexUniform < 0) {
		cerr << "Shader did not contain the 'texUnit' uniform." << endl;
	}
	mNormalAttribute = glGetAttribLocation(mShaderProgram, "normal");
	if (mNormalAttribute < 0) {
		cerr << "Shader did not contain the 'normal' attribute." << endl;
	}
	mPositionAttribute = glGetAttribLocation(mShaderProgram, "position");
	if (mPositionAttribute < 0) {
		cerr << "Shader did not contain the 'position' attribute." << endl;
	}
	mTexCoordAttribute = glGetAttribLocation(mShaderProgram, "texCoord");
	if (mTexCoordAttribute < 0) {
		cerr << "Shader did not contain the 'texCoord' attribute." << endl;
	}
	mRadiusAttribute = glGetAttribLocation(mShaderProgram, "radius");
	if (mRadiusAttribute < 0) {
		cerr << "Shader did not contain the 'radius' attribute." << endl;
	}
	mColorAttribute = glGetAttribLocation(mShaderProgram, "color");
	if (mColorAttribute < 0) {
		cerr << "Shader did not contain the 'color' attribute." << endl;
	}
}

