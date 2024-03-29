
#include "MyTractsKnot.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader/Shader.h"
#include "MyUtility.h"
#include "MyGraphicsTool.h"
#include "MyPrimitiveDrawer.h"
#include "MyBoxKnot.h"
#include "MyGlobalVariables.h"
#include <iostream>
using namespace std;

MyTractsKnot::MyTractsKnot(void)
{
	mFaces = 8;
	mDrawBoundingBox = false;
}


MyTractsKnot::~MyTractsKnot(void)
{
}

void MyTractsKnot::Show(){
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	//MyGraphicsTool::Sphere(10);

	MyBoundingBox box[2];
	for (int i = 0; i < mChildren.size(); i++){
		MyBoxKnot* knot = dynamic_cast<MyBoxKnot*>(mChildren[i]);
		if (knot){
			//knot->Show();
			box[i] = knot->GetBox();
		}
	}

	MyGraphicsTool::Translate(-mTracts->GetBoundingBox().GetCenter());

	glEnable(GL_DEPTH_TEST);
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

	int boxLowLocation1 = glGetUniformLocation(mShaderProgram, "boxLow1");
	int boxHighLocation1 = glGetUniformLocation(mShaderProgram, "boxHigh1");
	MyVec3f lowPos1 = box[0].GetLowPos()+mTracts->GetBoundingBox().GetCenter();
	MyVec3f highPos1 = box[0].GetHighPos() + mTracts->GetBoundingBox().GetCenter();
	glUniform3f(boxLowLocation1, lowPos1[0], lowPos1[1], lowPos1[2]);
	glUniform3f(boxHighLocation1, highPos1[0], highPos1[1], highPos1[2]);

	int boxLowLocation2 = glGetUniformLocation(mShaderProgram, "boxLow2");
	int boxHighLocation2 = glGetUniformLocation(mShaderProgram, "boxHigh2");
	MyVec3f lowPos2 = box[1].GetLowPos() + mTracts->GetBoundingBox().GetCenter();
	MyVec3f highPos2 = box[1].GetHighPos() + mTracts->GetBoundingBox().GetCenter();
	glUniform3f(boxLowLocation2, lowPos2[0], lowPos2[1], lowPos2[2]);
	glUniform3f(boxHighLocation2, highPos2[0], highPos2[1], highPos2[2]);

	int screenSpaceLocation = glGetUniformLocation(mShaderProgram, "screenSpace");
	int screenSpace = mbScreenSpace ? 1 : 0;
	glUniform1i(screenSpaceLocation, screenSpace);

	glDrawElements(GL_TRIANGLES, mIndices.size() * 3, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
	glDrawElements(GL_LINES, mIndices.size() * 3, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	glBindVertexArray(0);

	//glDisable(GL_CULL_FACE);
	MyGraphicsTool::PopMatrix();

	// draw later to ensure it will be shown
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	for (int i = 0; i < mChildren.size(); i++){
		MyBoxKnot* knot = dynamic_cast<MyBoxKnot*>(mChildren[i]);
		if (knot){
			knot->Show();
		}
	}

	glDisable(GL_DEPTH_TEST);
	MyGraphicsTool::Color(MyColor4f(0, 0, 1, 1));
	for (int i = 0; i < 2; i++){
		//MyPrimitiveDrawer::DrawStrokeText(
		//	box[i].GetLowPos() + mTracts->GetBoundingBox().GetCenter(), 
		//	MyString(i + 1), MyVec3f(.3, .3, .3));
		MyPrimitiveDrawer::DrawBitMapTextLarge(
			box[i].GetLowPos(),MyString(i + 1), 0);
	}
	glEnable(GL_DEPTH_TEST);
	MyGraphicsTool::PopMatrix();
}
void MyTractsKnot::Destory(){
	SafeFreeObject(mTracts);
	glDeleteProgram(mShaderProgram);
	glDeleteVertexArrays(1, &mVertexArray);
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mNormalBuffer);
	glDeleteBuffers(1, &mTexCoordBuffer);
	glDeleteBuffers(1, &mRadiusBuffer);
	glDeleteBuffers(1, &mColorBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
}
bool MyTractsKnot::SeeAny(){
	//int maxNumItems = mLayout->GetGraph()->GetNumNodes() + mLayout->GetGraph()->GetNumEdges();
	//unsigned int* buffer = new unsigned int[2 * maxNumItems];
	unsigned int buffer[2000];
	MyGraphicsTool::InitHitBuffer(2000, buffer);
	MyGraphicsTool::InitNameStack();
	MyGraphicsTool::ToSelectMode();
	//this->Show();
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	MyGraphicsTool::PushName(2);
	MyGraphicsTool::PushName(0);
	MyGraphicsTool::Translate(-mTracts->GetBoundingBox().GetCenter());
	mTracts->GetBoundingBox().Show();
	MyGraphicsTool::PopName();
	MyGraphicsTool::PopName();
	MyGraphicsTool::PopMatrix();
	int nHits = MyGraphicsTool::ToRenderMode();
	if (nHits <= 0) return false;
	return buffer[0] > 0;
}

MyGenericNode* MyTractsKnot::MakeSeeWhat(){
	return 0;
}

void MyTractsKnot::SetTracts(const MyTracts* tracts){
	mTracts = tracts;
}

void MyTractsKnot::SetNumSegments(int n){
	if(n>=3){
		mFaces = n;
	}
}

void MyTractsKnot::Build(){
	ComputeGeometry();
	LoadShader();
	LoadBuffer();
}


float MyTractsKnot::mapToSize(float s){
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

void MyTractsKnot::ComputeGeometry(){
	int currentIdx = 0;
	mIdxOffset.clear();
	int totalPoints = mTracts->GetTotalNumVertices() * (mFaces+1);

	mVertices.resize(totalPoints);
	mNormals.resize(totalPoints);
	mTexCoords.resize(totalPoints);
	mRadius.resize(totalPoints);
	mColors.resize(totalPoints);

	MyArrayf valueStats = mTracts->GetValueStats();
	float minValue = valueStats[0];
	float maxValue = valueStats[2];

	// localized range
	MyVec3f center = mTracts->GetBoundingBox().GetCenter();
	std::swap(minValue, maxValue);
	mAvgValues.clear();
	for (int i = 0; i < mChildren.size(); i++){
		MyBoxKnot* boxKnot = dynamic_cast<MyBoxKnot*>(mChildren[i]);
		MyBoundingBox box = boxKnot->GetBox();
		box.Translate(center);
		MyArrayf tvalueStats = mTracts->GetValueStats(box);
		float tmin = tvalueStats[0];
		mAvgValues << tvalueStats[1];
		float tmax = tvalueStats[2];
		minValue = std::min(minValue, tmin);
		maxValue = std::max(maxValue, tmax);
	}
	if (minValue > maxValue){
		std::swap(minValue, maxValue);
	}
	mMinValue = minValue;
	mMaxValue = maxValue;

	float rangeValue = maxValue - minValue;

	for(int it = 0;it < mTracts->GetNumTracts(); it++){
		int npoints = mTracts->GetNumVertices(it);
	
		const float PI = 3.1415926f;
		float dangle = 2*PI/mFaces;
		MyVec3f pole(0.6,0.8,0);

		MyArray3f candicates;
		candicates	<< MyVec3f(0,0,1)
					<< MyVec3f(0,1,1)
					<< MyVec3f(0,1,0)
					<< MyVec3f(1,1,0)
					<< MyVec3f(1,0,0)
					<< MyVec3f(1,0,1);
		float max = -1;
		int maxIdx;
		MyVec3f genDir = mTracts->GetCoord(it,0)-mTracts->GetCoord(it,npoints-1);
		genDir.normalize();
		for(int i = 0;i<candicates.size();i++){
			float cp = (candicates[i].normalized()^genDir).norm();
			if(cp>max){
				max = cp;
				maxIdx = i;
			}
		}
		pole = candicates[maxIdx].normalized();
	
		for(int i = 0;i<npoints;i++){
			MyVec3f p = mTracts->GetCoord(it,i);
			float size = (mTracts->GetValue(it, i) - minValue) / rangeValue;
			size = mapToSize(size);
			//float size = 0;
			MyVec3f d;
			if(i==npoints-1){
				d = p-mTracts->GetCoord(it,i-1);
			}
			else if(i == 0){
				d = mTracts->GetCoord(it,i+1)-p;
			}
			else{
				d = mTracts->GetCoord(it,i+1)-mTracts->GetCoord(it,i-1);
			}
		
			MyVec3f perpend1 = (pole^d).normalized();
			MyVec3f perpend2 = (perpend1^d).normalized();
			//if ((perpend1^perpend2)*d < 0) dangle = -dangle;
			for(int is = 0;is<mFaces;is++){
				float angle = dangle*is;
				MyVec3f pt = sin(angle)*perpend1 + cos(angle)*perpend2;
				mVertices[currentIdx+i*(mFaces+1)+is] = pt*0 + p;
				mNormals[currentIdx+i*(mFaces+1)+is] = pt;
				mTexCoords[currentIdx + i*(mFaces + 1) + is] = MyVec2f(i, is / (float)mFaces);
				mRadius[currentIdx + i*(mFaces + 1) + is] =  size;
				//mColors[currentIdx + i*(mFaces + 1) + is] = mTracts->GetColor(it, i);
				mColors[currentIdx + i*(mFaces + 1) + is] = MyColor4f(0.5,0.5,0.5,1);
			}
			mVertices[currentIdx+i*(mFaces+1)+mFaces] = mVertices[currentIdx+i*(mFaces+1)];
			mNormals[currentIdx+i*(mFaces+1)+mFaces] = mNormals[currentIdx+i*(mFaces+1)];
			mTexCoords[currentIdx + i*(mFaces + 1) + mFaces] = MyVec2f(i, 1);
			mRadius[currentIdx + i*(mFaces + 1) + mFaces] = mRadius[currentIdx + i*(mFaces + 1)];
			//mColors[currentIdx + i*(mFaces + 1) + mFaces] = mTracts->GetColor(it, i);
			mColors[currentIdx + i*(mFaces + 1) + mFaces] = MyColor4f(0.5, 0.5, 0.5, 1);
		}

		mIdxOffset << currentIdx;
		currentIdx += npoints*(mFaces+1);
	}

	// index
	for(int it = 0;it<mTracts->GetNumTracts();it++){
		int offset = mIdxOffset[it];
		for(int i = 1;i<mTracts->GetNumVertices(it);i++){
			for(int j = 0;j<=mFaces;j++){
				mIndices << MyVec3i(	(i-1)*(mFaces+1)+j%(mFaces+1) + offset,
								(i)*(mFaces+1)+j%(mFaces+1) + offset,
								(i)*(mFaces+1)+(j+1)%(mFaces+1) + offset);
				mIndices << MyVec3i(	(i-1)*(mFaces+1)+j%(mFaces+1) + offset,
								(i)*(mFaces+1)+(j+1)%(mFaces+1) + offset,
								(i-1)*(mFaces+1)+(j+1)%(mFaces+1) + offset);
			}
		}
	}
}

void MyTractsKnot::LoadBuffer(){
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
    glVertexAttribPointer(mNormalAttribute  , 3, GL_FLOAT, GL_FALSE, 0, 0);
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


void MyTractsKnot::LoadShader(){
	glDeleteProgram(mShaderProgram);
	mShaderProgram = InitShader("shader//tube.vert",  "shader//tube.frag", "fragColour");

	mTexUniform  = glGetUniformLocation(mShaderProgram, "texUnit");
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

int MyTractsKnot::mousePressEventHandler(MyGenericEvent& eve){
	if (MyGenericEvent::MOUSE_WHEEL == eve.GetEventMouseKey()){
		if (eve.GetEventKeyState() == MyGenericEvent::KEY_UP){
			mTrackBall.ScaleMultiply(1.05);
		}
		else if (eve.GetEventKeyState() == MyGenericEvent::KEY_DOWN){
			mTrackBall.ScaleMultiply(1 / 1.05);
		}
		eve.Handled();
	}
	else{
		int _x = eve.GetMouseX();
		int _y = eve.GetMouseY();
		mTrackBall.StartMotion(_x, _y);
	}
	//eve.Handled();
	return 1;
}

int MyTractsKnot::mouseReleaseEventHandler(MyGenericEvent& eve){
	if (MyGenericEvent::MOUSE_WHEEL == eve.GetEventMouseKey()){
		if (eve.GetEventKeyState() == MyGenericEvent::KEY_UP){
			mTrackBall.ScaleMultiply(1.05);
		}
		else if (eve.GetEventKeyState() == MyGenericEvent::KEY_DOWN){
			mTrackBall.ScaleMultiply(1 / 1.05);
		}
		eve.Handled();
	}
	else{
		int _x = eve.GetMouseX();
		int _y = eve.GetMouseY();
		mTrackBall.EndMotion(_x, _y);
	}
	//eve.Handled();
	return 1;
}

int MyTractsKnot::mouseMoveEventHandler(MyGenericEvent& eve){
	MyGenericEvent::MyMouseKey _mouseKey = eve.GetEventMouseKey();
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	// the drag event
	if(eve.GetEventKeyState() == MyGenericEvent::KEY_DOWN){
		// the drag event requres it being answering
		if(!IsAnswering()) return 0;
		switch (_mouseKey){
			case MyGenericEvent::MOUSE_KEY_LEFT:
				mTrackBall.RotateMotion(_x, _y);
				eve.SetNeedRedraw();
				eve.Handled();
				break;
			case MyGenericEvent::MOUSE_KEY_MIDDLE:
				mTrackBall.TranslateMotion(_x, _y);
				eve.SetNeedRedraw();
				eve.Handled();
				break;
			case MyGenericEvent::MOUSE_KEY_RIGHT:
				//mTrackBall.ScaleMotion(_x, _y);
				//eve.SetNeedRedraw();
				//eve.Handled();
				break;
			default:
				break;
		}
	}

	return 1;
}

int MyTractsKnot::idleEventHandler(MyGenericEvent& eve){
	eve.Visited();
	return 0;
}

int MyTractsKnot::windowResizeEventHandler(MyGenericEvent& eve){
	int _w = eve.GetWindowWidth();
	int _h = eve.GetWindowHeight();
	mTrackBall.Reshape(_w,_h);
	//mRepresentation->UpdataLabel();
	eve.Visited();
	return 0;
}

void MyTractsKnot::responseBroadcast(const MyKnot* source, int type){
	switch(type){
	case MyKnot::BROADCAST_TYPE_NONE:
		break;
	case MyKnot::BROADCAST_TYPE_HIGHLIGHT_SELECTION:
		if(this != source){
			this->replaceSelections(source);
		}
		this->HighLightSelection();
		break;
	default:
		break;
	}
}