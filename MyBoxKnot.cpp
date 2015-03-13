#include "MyBoxKnot.h"
#include "MyGraphicsTool.h"
/*
C:\Users\GuohaoZhang\Desktop\TmpFolder\Ranking\ranking_home\alignedboundingbox.cpp
*/
MyBoxKnot::MyBoxKnot()
{
}


MyBoxKnot::~MyBoxKnot()
{
}

void MyBoxKnot::SetBox(const MyBoundingBox& box){
	mBox = box;
}

const MyBoundingBox& MyBoxKnot::GetBox() const{
	return mBox;
}

void MyBoxKnot::Build(){
	MyArray3f vertices(8);
	MyArray4i faces(6);
	MyArray3f normals(6);
	MyArray2i edges(12);
	MyVec3f higherEnd = mBox.GetHighPos();
	MyVec3f lowerEnd = mBox.GetLowPos();

	vertices[0] = MyVec3f(higherEnd[0], lowerEnd[1], lowerEnd[2]);
	vertices[1] = MyVec3f(higherEnd[0], higherEnd[1], lowerEnd[2]);
	vertices[2] = MyVec3f(higherEnd[0], higherEnd[1], higherEnd[2]);
	vertices[3] = MyVec3f(higherEnd[0], lowerEnd[1], higherEnd[2]);
	vertices[4] = MyVec3f(lowerEnd[0], lowerEnd[1], lowerEnd[2]);
	vertices[5] = MyVec3f(lowerEnd[0], higherEnd[1], lowerEnd[2]);
	vertices[6] = MyVec3f(lowerEnd[0], higherEnd[1], higherEnd[2]);
	vertices[7] = MyVec3f(lowerEnd[0], lowerEnd[1], higherEnd[2]);

	// front
	faces[0] = MyVec4i(0, 1, 2, 3);
	// back
	faces[1] = MyVec4i(4, 7, 6, 5);
	// left
	faces[2] = MyVec4i(0, 3, 7, 4);
	// right
	faces[3] = MyVec4i(5, 6, 2, 1);
	// top
	faces[4] = MyVec4i(2, 6, 7, 3);
	// bottom
	faces[5] = MyVec4i(0, 4, 5, 1);

	for (int i = 0; i<6; i++){
		MyVec3f& a = vertices[faces[i][0]];
		MyVec3f& b = vertices[faces[i][1]];
		MyVec3f& c = vertices[faces[i][2]];
		normals[i] = ((b - a) ^ (c - a)).normalized();
	}

	edges[0][0] = 0;  edges[0][1] = 1;
	edges[1][0] = 1;  edges[1][1] = 2;
	edges[2][0] = 2;  edges[2][1] = 3;
	edges[3][0] = 3;  edges[3][1] = 0;
	edges[4][0] = 4;  edges[4][1] = 5;
	edges[5][0] = 5;  edges[5][1] = 6;
	edges[6][0] = 6;  edges[6][1] = 7;
	edges[7][0] = 7;  edges[7][1] = 4;
	edges[8][0] = 0;  edges[8][1] = 4;
	edges[9][0] = 1;  edges[9][1] = 5;
	edges[10][0] = 2; edges[10][1] = 6;
	edges[11][0] = 3; edges[11][1] = 7;

	MyGraphicsTool::DeleteList(mDisplayList);
	MyGraphicsTool::StartList(mDisplayList);

	MyGraphicsTool::EnableFaceCulling();
	// cull front face only
	MyGraphicsTool::FaceCullingMode(2);
	MyGraphicsTool::Color(MyColor4f(0.8f, 0.8f, 0.8f, 1.f));
	for (int iface = 0; iface<6; iface++){
		MyVec4i vidx = faces[iface];
		MyVec3f normal = normals[iface];
		MyGraphicsTool::BeginQuads();
		for (int ivert = 0; ivert < 4; ivert++){
			MyGraphicsTool::Normal(normal);
			MyGraphicsTool::Vertex(vertices[vidx[ivert]]);
		}
		MyGraphicsTool::EndPrimitive();
	}
	MyGraphicsTool::DisableFaceCulling();

	MyGraphicsTool::SetLineWidth(3);
	MyGraphicsTool::Color(MyColor4f(1, 1, 0, 1));
	for (int iedge = 0; iedge<12; iedge++){
		int vidx1 = edges[iedge][0];
		int vidx2 = edges[iedge][1];
		MyGraphicsTool::BeginLines();
		MyGraphicsTool::Vertex(vertices[vidx1]);
		MyGraphicsTool::Vertex(vertices[vidx2]);
		MyGraphicsTool::EndPrimitive();
	}

	MyGraphicsTool::SetLineWidth(1);
	MyGraphicsTool::EndList(mDisplayList);
}

void MyBoxKnot::Show(){
	MyGraphicsTool::PushMatrix();
	MyGraphicsTool::LoadTrackBall(&mTrackBall);
	MyGraphicsTool::ShowList(mDisplayList);
	MyGraphicsTool::PopMatrix();
}

void MyBoxKnot::Destory(){
	MyGraphicsTool::DeleteList(mDisplayList);
}

bool MyBoxKnot::SeeAny(){
	return false;
}

MyGenericNode* MyBoxKnot::MakeSeeWhat(){
	return 0;
}


int MyBoxKnot::mousePressEventHandler(MyGenericEvent& eve){
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

int MyBoxKnot::mouseReleaseEventHandler(MyGenericEvent& eve){
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

int MyBoxKnot::mouseMoveEventHandler(MyGenericEvent& eve){
	MyGenericEvent::MyMouseKey _mouseKey = eve.GetEventMouseKey();
	int _x = eve.GetMouseX();
	int _y = eve.GetMouseY();
	// the drag event
	if (eve.GetEventKeyState() == MyGenericEvent::KEY_DOWN){
		// the drag event requres it being answering
		if (!IsAnswering()) return 0;
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

int MyBoxKnot::idleEventHandler(MyGenericEvent& eve){
	eve.Visited();
	return 0;
}

int MyBoxKnot::windowResizeEventHandler(MyGenericEvent& eve){
	int _w = eve.GetWindowWidth();
	int _h = eve.GetWindowHeight();
	mTrackBall.Reshape(_w, _h);
	eve.Visited();
	return 0;
}

void MyBoxKnot::responseBroadcast(const MyKnot* source, int type){
	switch (type){
	case MyKnot::BROADCAST_TYPE_NONE:
		break;
	case MyKnot::BROADCAST_TYPE_HIGHLIGHT_SELECTION:
		if (this != source){
			this->replaceSelections(source);
		}
		this->HighLightSelection();
		break;
	default:
		break;
	}
}