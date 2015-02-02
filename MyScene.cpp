#include "MyScene.h"
#include "MyView.h"
#include "MyGraphicsTool.h"
#include "MyUtility.h"
#include "MySelectionEvent.h"

#include <iostream>
#include <vector>

MyScene::MyScene(void)
{
	mLayoutManager.SetKnotContainer(&mKnotContainer);
	mInteractionMode = 0;
	mView = 0;
}


MyScene::~MyScene(void)
{
}

void MyScene::SetView(MyView* view){
	mView = view;
}

void MyScene::Show(){
	int nKnots = mKnotContainer.GetNumberKnots();
	for (int i = 0; i < nKnots; i++){
		MyKnot* knot1 = mKnotContainer.GetKnot(i);
		knot1->Show();

		MyVec3f center1 = knot1->GetTrackBall().GetTotalTranslation();
		for (int j = i + 1; j < nKnots; j++){
			MyKnot* knot2 = mKnotContainer.GetKnot(j);
			if (knot1->HasDescendantNode(knot2)
				|| knot2->HasDescendantNode(knot1)){
				MyVec3f center2 = knot2->GetTrackBall().GetTotalTranslation();

				MyGraphicsTool::Color(MyColor4f::white());
				MyGraphicsTool::BeginLines();
				MyGraphicsTool::Vertex(center1);
				MyGraphicsTool::Vertex(center2);
				MyGraphicsTool::EndPrimitive();
			}
		}
	}
}

int MyScene::AddKnot(MyKnot* pKnot){
	if (mView){
		const MyMatrixf mat = mView->GetTrackBall().Matrix();
		MyVec3f center(-mat.At(0, 3), mat.At(1, 3), -930);
		pKnot->SetToDefault();
		pKnot->Scale(3);
		pKnot->Translate(center);
	}
	return mKnotContainer.AddKnot(pKnot);
}

int MyScene::RemoveKnot(MyKnot* pKnot){
	return mKnotContainer.RemoveKnot(pKnot);
}

int MyScene::RemoveKnots(const MyArray<MyKnot*>& pKnots){
	int rst = 0;
	for (int i = 0; i < pKnots.size(); i++){
		rst += mKnotContainer.RemoveKnot(pKnots[i]);
	}
	return rst;
}

int MyScene::EventHandler(MyGenericEvent& eve){
	mView->SetScene(this);

	if(eve.GetEventMouseKey() == MyGenericEvent::MOUSE_KEY_LEFT
		&& eve.GetEventKeyState() == MyGenericEvent::KEY_DOWN
		&& eve.GetEventSource() == MyGenericEvent::MOUSE_KEY){
		if (eve.GetModiferState().IsCtrlActive()){
			MyKnot* knot = mView->GetKnotAt(eve.GetMouseX(), eve.GetMouseY());
			if (knot){
				knot->EventHandler(eve);
				if (mFocusKnots.HasOne(knot)){
					mFocusKnots.EraseOne(knot);
				}
				else{
					mFocusKnots << knot;
				}
				eve.Handled();
				this->hintFocusKnot();
				return 1;
			}
		}
		else{
			mFocusKnots.clear();
			MyKnot* knot = mView->GetKnotAt(eve.GetMouseX(), eve.GetMouseY());
			if (knot){
				knot->EventHandler(eve);
				mFocusKnots << knot;
				eve.Handled();
			}
			// if get a hit, return
			this->hintFocusKnot();
			return 1;
		}
	}
	int nHandle = 0;
	if(mFocusKnots.size()>0){
		nHandle += mFocusKnots[0]->EventHandler(eve);
	}
	for(int i = 0;i<mLayoutManager.GetNumKnots();i++){
		MyKnot* knot = mKnotContainer.GetKnot(i);
		if (mFocusKnots.size()>0){
			if (knot != mFocusKnots[0]){
				nHandle += knot->EventHandler(eve);
			}
		}
		else{
			nHandle += knot->EventHandler(eve);
		}
	}
	Clamp(nHandle,0,1);
	return nHandle;
}

void MyScene::Build(){
	mLayoutManager.Update();
}

void MyScene::Link(const MyArray<MyKnot*>& pKnots){
	for (int i = 1; i < pKnots.size(); i++){
		if (pKnots[i - 1] < pKnots[i]){
			MyGenericNode::Connect(pKnots[i - 1], pKnots[i]);
		}
		else{
			MyGenericNode::Connect(pKnots[i], pKnots[i-1]);
		}
	}
}

void MyScene::Delink(const MyArray<MyKnot*>& pKnots){
	for (int i = 1; i < pKnots.size(); i++){
		MyGenericNode::Disconnect(pKnots[i - 1], pKnots[i]);
		MyGenericNode::Disconnect(pKnots[i], pKnots[i-1]);
	}
}

MyKnot* MyScene::GetKnot(int i){
	return mKnotContainer.GetKnot(i);
}
const MyKnot* MyScene::GetKnot(int i) const{
	return mLayoutManager.GetKnot(i);
}
int MyScene::GetNumKnots() const{
	return mKnotContainer.GetNumberKnots();
}

MyArray<MyKnot*> MyScene::GetFocusKnots() const{
	return mFocusKnots;
}

/*
int MyScene::keyReleaseEventHandler(unsigned char, int, int){
};
int MyScene::keyPressEventHandler(unsigned char, int, int){
};
int MyScene::mousePressEventHandler(int, int , int){
};

int MyScene::mouseReleaeEventHandler(int, int , int){
};

int MyScene::mouseMoveEventHandler(int, int){
};
*/
void MyScene::hintFocusKnot(){
	for(int i = 0;i<mLayoutManager.GetNumKnots();i++){
		mKnotContainer.GetKnot(i)->SetHighlightLevel(0);
		mKnotContainer.GetKnot(i)->ReadyToListen();
	}
	if (mFocusKnots.size()>0){
		mFocusKnots[0]->SetHighlightLevel(1);
		mFocusKnots[0]->StartToAnswer();
	}
	for (int i = 1; i<mFocusKnots.size(); i++){
		mFocusKnots[i]->SetHighlightLevel(2);
	}
}