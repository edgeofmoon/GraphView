#include "MyWindow.h"
#include "MyGraphicsTool.h"

MyWindow::MyWindow(void)
{
	mMapHeightRatio = 0.0;
	mFocusView = 0;
}


MyWindow::~MyWindow(void)
{
}

void MyWindow::Show(){
	for(int i = 0;i<mViews.size();i++){
		mViews[i]->Show();
	}
	/*
	MyGraphicsTool::SetViewport(MyVec4i(0,0,mWidth,mHeight));
	MyGraphicsTool::PushProjectionMatrix();
	MyGraphicsTool::LoadProjectionMatrix(&MyMatrixf::IdentityMatrix());
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadModelViewMatrix(&MyMatrixf::IdentityMatrix());
	MyGraphicsTool::SetLineWidth(2);
	MyGraphicsTool::Color(MyColor4f::black());
	MyGraphicsTool::BeginLines();
	MyGraphicsTool::Vertex(MyVec3f(-1,0.8,0));
	MyGraphicsTool::Vertex(MyVec3f(1,0.8,0));
	MyGraphicsTool::EndPrimitive();
	MyGraphicsTool::SetLineWidth(1);
	MyGraphicsTool::Color(MyColor4f::white());
	MyGraphicsTool::BeginLines();
	MyGraphicsTool::Vertex(MyVec3f(-1,0.8,0));
	MyGraphicsTool::Vertex(MyVec3f(1,0.8,0));
	MyGraphicsTool::EndPrimitive();
	MyGraphicsTool::PopMatrix();
	MyGraphicsTool::PopProjectionMatrix();
	*/
}

void MyWindow::AddView(MyView* view){
	mViews << view;
}
	
int MyWindow::EventHandler(MyGenericEvent& eve){
	int handleResult = 0;
	handleResult += MyGenericEventListener::EventHandler(eve);
	mFocusView = mViews[0];
	if(mFocusView){
		handleResult += mFocusView->EventHandler(eve);
	}
	for(int i = 0;i<mViews.size() && eve.GetEventHandleState() != MyGenericEvent::EVENT_HANDLED;i++){
		if(mViews[i] != mFocusView){
			handleResult += mViews[i]->EventHandler(eve);
		}
	}
	return handleResult;
}

int MyWindow::windowResizeEventHandler(MyGenericEvent& eve){
	mWidth = eve.GetWindowWidth();
	mHeight = eve.GetWindowHeight();
	MyGraphicsTool::SetSize(mWidth,mHeight);
	mViews[0]->SetViewport(MyVec4i(0,0, mWidth, mHeight));
	return 1;
}