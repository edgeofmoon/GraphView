#include "MyView.h"
#include "MyGraphicsTool.h"
#include "MyScene.h"
#include "MyGenericHierarchyOverlay.h"

MyView::MyView(void)
{
	mScene = 0;
	mTrackBall.Reshape(1024,768);
	mViewport = MyVec4i(0,0,1024,768);
}

MyView::MyView(MyScene* scene){
	mScene = 0;
	this->SetScene(mScene);
}

MyView::~MyView(void)
{
}

void MyView::Show(){
	
	MyGraphicsTool::SetViewport(mViewport);
	this->LoadView();
	//this->LoadPeepholeView(512,384,50,50);
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	mScene->Show();
}

void MyView::SetScene(MyScene* scene){
	mScene = scene;
}

MyKnot* MyView::GetKnotAt(int px, int py){
	this->LoadPeepholeView(px, py);
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	for (int i = 0; i < mScene->GetNumKnots(); i++){
		MyKnot* knot = mScene->GetKnot(i);
		if (knot->SeeAny()){
			return knot;
		}
	}
	return 0;
}

MyGenericNode* MyView::MakeSeeWhat(int px, int py, MyKnot* knot){
	MyGenericNode* rst = 0;
	this->LoadPeepholeView(px, py, 1, 1);
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	if(knot){
		if (knot->SeeAny()){
			rst = knot->MakeSeeWhat();
			return rst;
		}
		return 0;
	}
	else{
		for (int i = 0; i < mScene->GetNumKnots(); i++){
			MyKnot* tknot = mScene->GetKnot(i);
			if (tknot->SeeAny()){
				rst = tknot->MakeSeeWhat();
				return rst;
			}
		}
	}
	return 0;
}

void MyView::SetViewport(const MyVec4i &viewport){
	mViewport = viewport;
}

void MyView::Build(){
	//mWidth = 1024;
	//mHeight = 768;
	mWidth = mViewport[2];
	mHeight = mViewport[3];
	//mProjectionMatrix = MyMatrixf::PerspectiveMatrix(60,mWidth/(float)mHeight,1,1000);
	mProjectionMatrix = MyMatrixf::OrthographicMatrix(
		-mWidth/2,mWidth/2,-mHeight/2,mHeight/2,0.001f,1000.f);
	mModelViewMatrix = MyMatrixf::IdentityMatrix();
	MyGraphicsTool::SetBackgroundColor(MyColor4f(0.5f,0.5f,0.5f,0.f));
}
void MyView::LoadView(){
	MyGraphicsTool::LoadProjectionMatrix(&mProjectionMatrix);
	MyGraphicsTool::LoadModelViewMatrix(&mModelViewMatrix);
}
void MyView::LoadLeftEyeView(){
}
void MyView::LoadRightEyeView(){
}
// peep through this pixel
// used to pick at cursor
void MyView::LoadPeepholeView(int x, int y, int w, int h){
	MyGraphicsTool::LoadPickMatrix(x, y, w, h, &mProjectionMatrix,mViewport);
	MyGraphicsTool::LoadModelViewMatrix(&mModelViewMatrix);
}

int MyView::EventHandler(MyGenericEvent& eve){
	mScene->SetView(this);
	int sceneHandleResult = mScene->EventHandler(eve);
	if (eve.GetEventHandleState() != MyGenericEvent::EVENT_HANDLED){
		return MyGenericEventListener::EventHandler(eve);
	}
	return sceneHandleResult;
}

MyTrackBall& MyView::GetTrackBall(){
	return mTrackBall;
}

int MyView::keyPressEventHandler(MyGenericEvent& eve){
	for(int i = 0;i<mScene->GetNumKnots();i++){
		eve.SetEventHandleState(MyGenericEvent::EVENT_VISITED);
		mScene->GetKnot(i)->StartToAnswer();
		mScene->GetKnot(i)->EventHandler(eve);
		mScene->GetKnot(i)->ReadyToListen();
	}
	return 1;
}

int MyView::mousePressEventHandler(MyGenericEvent& eve){
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	mTrackBall.StartMotion(_x, _y);
	eve.Handled();
	return 1;
}

int MyView::mouseReleaseEventHandler(MyGenericEvent& eve){
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	mTrackBall.EndMotion(_x, _y);
	eve.Handled();
	return 1;
}

int MyView::mouseMoveEventHandler(MyGenericEvent& eve){
	MyGenericEvent::MyMouseKey _mouseKey = eve.GetEventMouseKey();
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	float scaleIdx;
	switch (_mouseKey){
	case MyGenericEvent::MOUSE_KEY_LEFT:
		//mTrackBall.RotateMotion(x, y);
		eve.Handled();
		break;
	case MyGenericEvent::MOUSE_KEY_MIDDLE:
		mTrackBall.TranslateMotionX(_x, _y);
		eve.SetNeedRedraw();
		eve.Handled();
		break;
	case MyGenericEvent::MOUSE_KEY_RIGHT:
		//mTrackBall.ZoomMotion(x, y);
		// scale all
		//scaleIdx = mTrackBall.GetScaleFromMotion(_x, _y);
		//for(int i = 0;i<mScene->GetNumKnots();i++){
		//	mScene->GetKnot(i)->Scale(scaleIdx);
		//}
		//eve.SetNeedRedraw();
		break;
	default:
		break;

	}
	return 1;
}

int MyView::windowResizeEventHandler(MyGenericEvent& eve){
	mWidth = mViewport[2];
	mHeight = mViewport[3];
	
	mProjectionMatrix = MyMatrixf::PerspectiveMatrix(60,mWidth/(float)mHeight,1,10000);
	
	//mProjectionMatrix = MyMatrixf::OrthographicMatrix(
	//	-mWidth / 2, mWidth / 2, -mHeight / 2, mHeight / 2, 1.f, 1000.f);
	mTrackBall.Reshape(mWidth,mHeight);
	int sceneHandleResult = mScene->EventHandler(eve);
	return sceneHandleResult;
}

int MyView::idleEventHandler(MyGenericEvent& eve){
	int sceneHandleResult = mScene->EventHandler(eve);
	return sceneHandleResult;
}