
#include "MyTractsKnot.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader/Shader.h"
#include "MyUtility.h"
#include "MyGraphicsTool.h"
#include "MyPrimitiveDrawer.h"
#include <iostream>
using namespace std;

MyTractsKnot::MyTractsKnot(void)
{
	mFaces = 6;
	mRadius = 0.4;
	mDrawBoundingBox = false;
}


MyTractsKnot::~MyTractsKnot(void)
{
}

void MyTractsKnot::Show(){
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	//MyGraphicsTool::Sphere(10);
	MyGraphicsTool::Translate(-mTracts->GetBoundingBox().GetCenter());

	
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

	glDrawElements(GL_TRIANGLES, mIndices.size()*3, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	glBindVertexArray(0);
	
	MyGraphicsTool::PopMatrix();
}
void MyTractsKnot::Destory(){
	SafeFreeObject(mTracts);
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

void MyTractsKnot::ComputeGeometry(){
	int currentIdx = 0;
	mIdxOffset.clear();
	int totalPoints = mTracts->GetTotalNumVertices() * (mFaces+1);

	mVertices.resize(totalPoints);
	mNormals.resize(totalPoints);
	mTexCoords.resize(totalPoints);
	mColors.resize(totalPoints);

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
		pole = candicates[maxIdx];
	
		for(int i = 0;i<npoints;i++){
			MyVec3f p = mTracts->GetCoord(it,i);
			float size = mRadius;
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
		
			MyVec3f perpend = (pole^d).normalized();
			for(int is = 0;is<mFaces;is++){
				float angle = dangle*is;
				MyVec3f pt = sin(angle)*pole+cos(angle)*perpend;
				mVertices[currentIdx+i*(mFaces+1)+is] = pt*size + p;
				mNormals[currentIdx+i*(mFaces+1)+is] = pt;
				mTexCoords[currentIdx+i*(mFaces+1)+is] = MyVec2f(i, is/(float)mFaces);
				mColors[currentIdx+i*(mFaces+1)+is] = mTracts->GetColor(it,i);
			}
			mVertices[currentIdx+i*(mFaces+1)+mFaces] = mVertices[currentIdx+i*(mFaces+1)];
			mNormals[currentIdx+i*(mFaces+1)+mFaces] = mNormals[currentIdx+i*(mFaces+1)];
			mTexCoords[currentIdx+i*(mFaces+1)+mFaces] = MyVec2f(i, 1);
			mColors[currentIdx+i*(mFaces+1)+mFaces] = mTracts->GetColor(it,i);
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
    glVertexAttribPointer(mTexCoordAttribute , 2, GL_FLOAT, GL_FALSE, 0, 0);
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
	mColorAttribute = glGetAttribLocation(mShaderProgram, "color");
	if (mColorAttribute < 0) {
		cerr << "Shader did not contain the 'color' attribute." << endl;
	}
}

int MyTractsKnot::mousePressEventHandler(MyGenericEvent& eve){
	if (MyGenericEvent::MOUSE_WHEEL == eve.GetEventMouseKey()){
		if (eve.GetEventKeyState() == MyGenericEvent::KEY_UP){
			mTrackBall.ScaleAdd(0.05);
		}
		else if (eve.GetEventKeyState() == MyGenericEvent::KEY_DOWN){
			mTrackBall.ScaleAdd(-0.05);
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
			mTrackBall.ScaleAdd(0.05);
		}
		else if (eve.GetEventKeyState() == MyGenericEvent::KEY_DOWN){
			mTrackBall.ScaleAdd(-0.05);
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