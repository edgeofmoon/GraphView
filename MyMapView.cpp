#include "MyMapView.h"
#include "MyGraphicsTool.h"
#include "MyScene.h"

MyMapView::MyMapView(void)
{
}


MyMapView::~MyMapView(void)
{
}

void MyMapView::Build(){
	//mWidth = 1024;
	//mHeight = 768;
	mWidth = mViewport[2];
	mHeight = mViewport[3];
	//mProjectionMatrix = MyMatrixf::PerspectiveMatrix(60,mWidth/(float)mHeight,1,1000);
	mProjectionMatrix = MyMatrixf::OrthographicMatrix(
		-mWidth/2*10,mWidth/2*10,-mHeight/2*10,mHeight/2*10,1.f,10000.f);
	mModelViewMatrix = MyMatrixf::IdentityMatrix();
	MyGraphicsTool::SetBackgroundColor(MyColor4f(0.5f,0.5f,0.5f,0.f));
}

void MyMapView::Show(){
	MyGraphicsTool::SetViewport(mViewport);
	this->LoadView();
	mScene->Show();

	int x = -mView->GetTrackBall().Matrix().At(0,3);

	MyGraphicsTool::SetLineWidth(2);
	MyGraphicsTool::Color(MyColor4f::white());
	MyGraphicsTool::BeginLineLoop();
	MyGraphicsTool::Vertex(MyVec3f(x - mWidth / 2, -mHeight / 2 * 9, -1));
	MyGraphicsTool::Vertex(MyVec3f(x + mWidth / 2, -mHeight / 2 * 9, -1));
	MyGraphicsTool::Vertex(MyVec3f(x + mWidth / 2, mHeight / 2 * 9, -1));
	MyGraphicsTool::Vertex(MyVec3f(x - mWidth / 2, mHeight / 2 * 9, -1));
	MyGraphicsTool::EndPrimitive();


	MyGraphicsTool::SetLineWidth(1);
}

void MyMapView::SetView(MyView* view){
	mView = view;
}

int MyMapView::mousePressEventHandler(MyGenericEvent& eve){
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	mView->GetTrackBall().StartMotion(_x, _y);
	mView->GetTrackBall().TranslateTo(-_x * 10 + mWidth * 5, 0);
	eve.SetNeedRedraw();
	eve.Handled();
	return 1;
}

int MyMapView::mouseReleaseEventHandler(MyGenericEvent& eve){
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	mView->GetTrackBall().EndMotion(_x,_y);
	return 1;
}

int MyMapView::mouseMoveEventHandler(MyGenericEvent& eve){
	MyGenericEvent::MyMouseKey _mouseKey = eve.GetEventMouseKey();
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	float scaleIdx;
	switch (_mouseKey){
	case MyGenericEvent::MOUSE_KEY_LEFT:
		mView->GetTrackBall().TranslateTo(-_x * 10 + mWidth * 5, 0);
		eve.SetNeedRedraw();
		eve.Handled();
		break;
	case MyGenericEvent::MOUSE_KEY_MIDDLE:
		//mView->GetTrackBall().TranslateMotionX(_x, _y, -30);
		mView->GetTrackBall().TranslateTo(-_x*10+mWidth*5, 0);
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

int MyMapView::windowResizeEventHandler(MyGenericEvent& eve){
	mWidth = mViewport[2];
	mHeight = mViewport[3];
	
	//mProjectionMatrix = MyMatrixf::PerspectiveMatrix(60,mWidth/(float)mHeight,1,1000);
	
	mProjectionMatrix = MyMatrixf::OrthographicMatrix(
		-mWidth/2*10,mWidth/2*10,-mHeight/2*10,mHeight/2*10,1.f,10000.f);
	mTrackBall.Reshape(mWidth,mHeight);
	int sceneHandleResult = mScene->EventHandler(eve);
	return sceneHandleResult;
}