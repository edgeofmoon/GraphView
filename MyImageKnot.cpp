#include "MyImageKnot.h"
#include "MyGraphicsTool.h"
#include "MyPrimitiveDrawer.h"
#include <GL/freeglut.h>

MyImageKnot::MyImageKnot(void)
{
	mImage = 0;
	mDisplayList = -1;
	mTexture = -1;
}


MyImageKnot::~MyImageKnot(void)
{
}

void MyImageKnot::SetImage(const MyImage* image){
	mImage = image;
}

void MyImageKnot::SetSize(int width, int height){
	mWidth = width;
	mHeight = height;
}

void MyImageKnot::Build(){
	if (!mImage){
		return;
	}
	float scale = 0.2;
	mWidth = mImage->GetWidth();
	mHeight = mImage->GetHeight();

	MyVec3f lowerLeft(-mWidth/2.f*scale,-mHeight/2.f*scale,0);
	MyVec3f lowerRight(mWidth/2.f*scale,-mHeight/2.f*scale,0);
	MyVec3f upperLeft(-mWidth/2.f*scale,mHeight/2.f*scale,0);
	MyVec3f upperRight(mWidth/2.f*scale,mHeight/2.f*scale,0);

	mBoundingBox.Reset();
	mBoundingBox.Engulf(lowerLeft);
	mBoundingBox.Engulf(lowerRight);
	mBoundingBox.Engulf(upperLeft);
	mBoundingBox.Engulf(upperRight);

	MyGraphicsTool::DeleteList(mDisplayList);
	MyGraphicsTool::DeleteTexture(mTexture);

	MyGraphicsTool::EnableTexture2D();
	mTexture = MyGraphicsTool::GenerateTexture();
	MyGraphicsTool::BindTexture2D(mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 
		0, GL_RGB, GL_UNSIGNED_BYTE, mImage->GetPixelBufferRGB());
	MyGraphicsTool::AutoSpecifyTexutreParamters();
	MyGraphicsTool::GenerateMipMap2D();
	
	MyGraphicsTool::StartList(mDisplayList);
	MyGraphicsTool::BeginTriangleFan();
	MyGraphicsTool::TextureCoordinate(MyVec2f(0,0));
	MyGraphicsTool::Vertex(lowerLeft);
	MyGraphicsTool::TextureCoordinate(MyVec2f(1,0));
	MyGraphicsTool::Vertex(lowerRight);
	MyGraphicsTool::TextureCoordinate(MyVec2f(1,1));
	MyGraphicsTool::Vertex(upperRight);
	MyGraphicsTool::TextureCoordinate(MyVec2f(0,1));
	MyGraphicsTool::Vertex(upperLeft);
	MyGraphicsTool::EndPrimitive();
	MyGraphicsTool::EndList(mDisplayList);

	MyGraphicsTool::UnbindTexture2D(mTexture);
	MyGraphicsTool::DisableTexture2D();
}

void MyImageKnot::Show(){
	float scale = 0.2;
	MyVec3f lowerLeft(-mWidth/2.f*scale,-mHeight/2.f*scale,0);
	MyVec3f lowerRight(mWidth/2.f*scale,-mHeight/2.f*scale,0);
	MyVec3f upperLeft(-mWidth/2.f*scale,mHeight/2.f*scale,0);
	MyVec3f upperRight(mWidth/2.f*scale,mHeight/2.f*scale,0);

	MyGraphicsTool::Color(MyColor4f::white());
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	MyGraphicsTool::EnableTexture2D();
	MyGraphicsTool::BindTexture2D(mTexture);
	MyGraphicsTool::ShowList(mDisplayList);
	MyGraphicsTool::UnbindTexture2D(mTexture);
	MyGraphicsTool::DisableTexture2D();
	
	if (mHighlightLevel>0){
		MyGraphicsTool::Color(MyColor4f::white());
		if (mHighlightLevel == 1){
			MyGraphicsTool::SetLineWidth(3);
		}
		else{
			MyGraphicsTool::SetLineWidth(1);
		}
		MyGraphicsTool::WirePolygon();
		mBoundingBox.Show();
		MyGraphicsTool::FillPolygon();
		MyGraphicsTool::SetLineWidth(1);
	}

	MyPrimitiveDrawer::DrawBitMapTextLarge(mBoundingBox.GetBottomCenter() - MyVec3f(0, 5, 0), mName);
	//MyGraphicsTool::Sphere(30);

	MyGraphicsTool::PopMatrix();

}

void MyImageKnot::Destory(){
	MyGraphicsTool::DeleteList(mDisplayList);
	MyGraphicsTool::DeleteTexture(mTexture);
}

bool MyImageKnot::SeeAny(){
	unsigned int buffer[2000];
	MyGraphicsTool::InitHitBuffer(2000, buffer);
	MyGraphicsTool::InitNameStack();
	MyGraphicsTool::ToSelectMode();
	//this->Show();
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	MyGraphicsTool::PushName(2);
	MyGraphicsTool::PushName(0);
	MyGraphicsTool::Translate(mBoundingBox.GetCenter());
	mBoundingBox.Show();
	MyGraphicsTool::PopName();
	MyGraphicsTool::PopName();
	MyGraphicsTool::PopMatrix();
	int nHits = MyGraphicsTool::ToRenderMode();
	if (nHits <= 0) return false;
	return buffer[0] > 0;
}

MyGenericNode* MyImageKnot::MakeSeeWhat(){
	return 0;
}


int MyImageKnot::mousePressEventHandler(MyGenericEvent& eve){
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

int MyImageKnot::mouseReleaseEventHandler(MyGenericEvent& eve){
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

int MyImageKnot::mouseMoveEventHandler(MyGenericEvent& eve){
	MyGenericEvent::MyMouseKey _mouseKey = eve.GetEventMouseKey();
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	// the drag event
	if(eve.GetEventKeyState() == MyGenericEvent::KEY_DOWN){
		// the drag event requres it being answering
		if(!IsAnswering()) return 0;
		switch (_mouseKey){
			case MyGenericEvent::MOUSE_KEY_LEFT:
				//mTrackBall.RotateMotion(_x, _y);
				//eve.SetNeedRedraw();
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

int MyImageKnot::idleEventHandler(MyGenericEvent& eve){
	eve.Visited();
	return 0;
}

int MyImageKnot::windowResizeEventHandler(MyGenericEvent& eve){
	int _w = eve.GetWindowWidth();
	int _h = eve.GetWindowHeight();
	mTrackBall.Reshape(_w,_h);
	//mRepresentation->UpdataLabel();
	eve.Visited();
	return 0;
}

void MyImageKnot::responseBroadcast(const MyKnot* source, int type){
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