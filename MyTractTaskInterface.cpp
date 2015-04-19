#include "MyTractTaskInterface.h"
#include "MyTractsKnot.h"
#include "MyUtility.h"
#include "MyPrimitiveDrawer.h"
#include "MyGlobalVariables.h"
#include "MySelectionEvent.h"

// positions
#define UI_BOX_LEFT 0.9
#define UI_BOX_BOTTOM -0.5
#define UI_BOX_SIZEY 0.1
#define UI_BOX_SIZEX 0.08
#define UI_BOX_INTERVAL 0.02
#define UI_BOX_FINISH_BOTTOM -0.8
#define UI_BOX_PAUSE_BOTTOM 0.8

// box text
#define UI_BOX_TEXT_LEFT_OFFSET (UI_BOX_SIZEX/2)
#define UI_BOX_TEXT_BOTTOM_OFFSET 0.04

// index
#define UI_BOX_ANSWER_INDEX_OFFSET 0

// 0: next 1: pause
#define UI_BOX_UTIL_INDEX_OFFSET 9000
#define UI_BOX_FINISH_INDEX 0
#define UI_BOX_PAUSE_INDEX 1

#define UI_BOX_FINISH_STRING ("NEXT")
#define UI_BOX_PAUSE_STRING ("PAUSE")
#define UI_BOX_RESUME_STRING ("RESUME")

// status, less more prior
#define UI_BOX_HIDDEN 0
#define UI_BOX_DISABLED 1
#define UI_BOX_READY 2
#define UI_BOX_SELECTED 3
#define UI_BOX_HOVER 4

// color
#define UI_BOX_DISABLED_COLOR MyColor4f(0.7f, 0.7f, 0.7f, 0.7f)
#define UI_BOX_READY_COLOR MyColor4f(1.0f, 1.0f, 1.0f, 1.0f)
#define UI_BOX_HOVER_COLOR MyColor4f(1.0f, 1.0f, 1.0f, 1.0f)
#define UI_BOX_SELECTED_COLOR MyColor4f(1.0f, 1.0f, 0.0f, 1.0f)

// line width
#define UI_BOX_DISABLED_WIDTH 1
#define UI_BOX_READY_WIDTH 1
#define UI_BOX_HOVER_WIDTH 2
#define UI_BOX_SELECTED_WIDTH 2

MyTractTaskInterface::MyTractTaskInterface()
{
	mUserSelection = -1;
	mShowResult = false;
	mFinished = false;
	mIsPaused = false;
	mEnabled = false;
	mEmpty = false;
}

MyTractTaskInterface::~MyTractTaskInterface()
{
}

void MyTractTaskInterface::Show(){
	MyMatrixf mat = MyMatrixf::IdentityMatrix();
	MyPrimitiveDrawer::PushProjectionMatrix();
	MyPrimitiveDrawer::PushMatrix();
	MyPrimitiveDrawer::LoadProjectionMatrix(&mat);
	MyPrimitiveDrawer::LoadModelViewMatrix(&mat);

	for (int i = 0; i < mAnswerBoxes.size(); i++){
		drawBox(mAnswerBoxes[i], mAnswerBoxStrings[i], mAnswerBoxStatus[i]);
	}

	for (int i = 0; i < mUtilBoxes.size(); i++){
		drawBox(mUtilBoxes[i], mUtilBoxStrings[i], mUtilBoxStatus[i]);
	}

	MyPrimitiveDrawer::PopProjectionMatrix();
	MyPrimitiveDrawer::PopMatrix();
}

void MyTractTaskInterface::Build(){
	initBoxes(mEmpty);
}

bool MyTractTaskInterface::IsAnswerSet() const{
	return (mUserSelection >= UI_BOX_ANSWER_INDEX_OFFSET 
		&& mUserSelection < UI_BOX_UTIL_INDEX_OFFSET);
}

int MyTractTaskInterface::GetHitBoxIndex(int mx, int my, int winWidth, int winHeight) const{
	float x = mx / (float)winWidth * 2 - 1;
	float y = (winHeight - my) / (float)winHeight * 2 - 1;
	MyVec2f p(x, y);
	for (int i = 0; i < mAnswerBoxes.size(); i++){
		if (mAnswerBoxes[i].IsIn(p) && acceptClick(mAnswerBoxStatus[i])){
			return UI_BOX_ANSWER_INDEX_OFFSET + i;
		}
	}

	for (int i = 0; i < mUtilBoxes.size(); i++){
		if (mUtilBoxes[i].IsIn(p)){
			return UI_BOX_UTIL_INDEX_OFFSET + i;
		}
	}

	return -1;
}

int MyTractTaskInterface::EventHandler(MyGenericEvent& eve){
	if (!mEnabled) return 0;
	else{
		return MyGenericEventListener::EventHandler(eve);
	}
}


void MyTractTaskInterface::initBoxes(bool bEmpty){
	MyBox2f box_left, box_right, box_finish, box_pause;
	box_right.Set(MyVec2f(UI_BOX_LEFT, UI_BOX_BOTTOM), 
		MyVec2f(UI_BOX_LEFT+UI_BOX_SIZEX, UI_BOX_BOTTOM+UI_BOX_SIZEY));
	box_left = box_right + MyVec2f(0, UI_BOX_SIZEY + UI_BOX_INTERVAL);
	mAnswerBoxes << box_left << box_right;
	mAnswerBoxStrings << "1" << "2";
	mAnswerBoxStatus << UI_BOX_READY << UI_BOX_READY;

	box_finish.Set(MyVec2f(UI_BOX_LEFT, UI_BOX_FINISH_BOTTOM),
		MyVec2f(UI_BOX_LEFT + UI_BOX_SIZEX, UI_BOX_FINISH_BOTTOM + UI_BOX_SIZEY));
	box_pause.Set(MyVec2f(UI_BOX_LEFT, UI_BOX_PAUSE_BOTTOM),
		MyVec2f(UI_BOX_LEFT + UI_BOX_SIZEX, UI_BOX_PAUSE_BOTTOM + UI_BOX_SIZEY));
	mUtilBoxes << box_finish << box_pause;
	mUtilBoxStrings << UI_BOX_FINISH_STRING << UI_BOX_PAUSE_STRING;
	mUtilBoxStatus << UI_BOX_DISABLED << UI_BOX_READY;
}

int MyTractTaskInterface::upgradeStatus(int status0, int status1){
	if (status0 == UI_BOX_SELECTED && status1 == UI_BOX_HOVER) return status0;
	return status1;
}

int MyTractTaskInterface::clearBoxStatus(int status){
	int cleared = 0;
	for (int i = 0; i < mUtilBoxStatus.size(); i++){
		if (mUtilBoxStatus[i] >= status){
			mUtilBoxStatus[i] = UI_BOX_READY;
			cleared++;
		}
	}
	bool answerSelected = false;
	for (int i = 0; i < mAnswerBoxStatus.size(); i++){
		if (mAnswerBoxStatus[i] >= status){
			mAnswerBoxStatus[i] = UI_BOX_READY;
			cleared++;
		}
		if (mAnswerBoxStatus[i] == UI_BOX_SELECTED){
			answerSelected = true;
		}
	}

	mUtilBoxStatus[UI_BOX_FINISH_INDEX] = 
		answerSelected ? UI_BOX_READY : UI_BOX_DISABLED;

	return cleared;
}

bool MyTractTaskInterface::acceptClick(int status) const{
	return status != UI_BOX_HIDDEN && UI_BOX_DISABLED != status;
}

bool MyTractTaskInterface::acceptHover(int status) const{
	return status != UI_BOX_HIDDEN && UI_BOX_DISABLED != status;
}

void MyTractTaskInterface::drawBox(const MyBox2f& box, const MyString& str, const int status){
	switch (status){
	case UI_BOX_DISABLED:
		MyGraphicsTool::SetLineWidth(UI_BOX_DISABLED_WIDTH);
		MyGraphicsTool::Color(UI_BOX_DISABLED_COLOR);
		break;
	case UI_BOX_READY:
		MyGraphicsTool::SetLineWidth(UI_BOX_READY_WIDTH);
		MyGraphicsTool::Color(UI_BOX_READY_COLOR);
		break;
	case UI_BOX_HOVER:
		MyGraphicsTool::SetLineWidth(UI_BOX_HOVER_WIDTH);
		MyGraphicsTool::Color(UI_BOX_HOVER_COLOR);
		break;
	case UI_BOX_SELECTED:
		MyGraphicsTool::SetLineWidth(UI_BOX_SELECTED_WIDTH);
		MyGraphicsTool::Color(UI_BOX_SELECTED_COLOR);
		break;
	default:
	case UI_BOX_HIDDEN:
		return;
		break;
	}

	MyVec2f bl = box.GetLowPos();
	MyVec2f tr = box.GetHighPos();
	MyVec2f br(tr[0], bl[1]);
	MyVec2f tl(bl[0], tr[1]);

	MyGraphicsTool::BeginLineLoop();
	MyGraphicsTool::Vertex(bl);
	MyGraphicsTool::Vertex(br);
	MyGraphicsTool::Vertex(tr);
	MyGraphicsTool::Vertex(tl);
	MyGraphicsTool::EndPrimitive();

	MyPrimitiveDrawer::DrawBitMapText(MyVec3f(bl[0] + UI_BOX_TEXT_LEFT_OFFSET,
		bl[1] + UI_BOX_TEXT_BOTTOM_OFFSET, 0), str, 1);

	// restore state
	MyGraphicsTool::SetLineWidth(1);
	MyGraphicsTool::Color(MyColor4f::white());
}

int MyTractTaskInterface::mouseReleaseEventHandler(MyGenericEvent& eve){
	return 0;
}

int MyTractTaskInterface::mouseMoveEventHandler(MyGenericEvent& eve){
	//MySelectionEvent& seleve = dynamic_cast<MySelectionEvent&>(eve);
	float mx = eve.GetMouseX();
	float my = eve.GetMouseY();
	int width = eve.GetWindowWidth();
	int height = eve.GetWindowHeight();
	int idx = this->GetHitBoxIndex(mx, my, width, height);
	if (clearBoxStatus(UI_BOX_HOVER) > 0){
		eve.SetNeedRedraw();
	};
	if (idx < 0) {
		return 0;
	}
	if (idx >= UI_BOX_UTIL_INDEX_OFFSET){
		int bid = idx - UI_BOX_UTIL_INDEX_OFFSET;
		if (acceptHover(mUtilBoxStatus[bid])){
			mUtilBoxStatus[bid] = upgradeStatus(mUtilBoxStatus[bid], UI_BOX_HOVER);
			eve.SetNeedRedraw();
			return 1;
		}
	}
	else if (idx >= UI_BOX_ANSWER_INDEX_OFFSET){
		int bid = idx - UI_BOX_ANSWER_INDEX_OFFSET;
		if (acceptHover(mAnswerBoxStatus[bid])){
			mAnswerBoxStatus[bid] = upgradeStatus(mAnswerBoxStatus[bid], UI_BOX_HOVER);
			eve.SetNeedRedraw();
			return 1;
		}
	}
	return 0;
}

int MyTractTaskInterface::mousePressEventHandler(MyGenericEvent& eve){
	//MySelectionEvent& seleve = dynamic_cast<MySelectionEvent&>(eve);
	if (eve.GetEventSource() == MyGenericEvent::MOUSE_KEY
		&& eve.GetEventMouseKey() == MyGenericEvent::MOUSE_KEY_LEFT){
		float mx = eve.GetMouseX();
		float my = eve.GetMouseY();
		int width = eve.GetWindowWidth();
		int height = eve.GetWindowHeight();
		int idx = this->GetHitBoxIndex(mx, my, width, height);
		if (idx < 0) return 0;
		eve.SetNeedRedraw();
		if (idx == UI_BOX_UTIL_INDEX_OFFSET + UI_BOX_PAUSE_INDEX){
			mIsPaused = !mIsPaused;
			if (mIsPaused){
				//seleve.SetActionName("UI_PAUSE");
				mUtilBoxStrings[UI_BOX_PAUSE_INDEX] = UI_BOX_RESUME_STRING;
			}
			else{
				//seleve.SetActionName("UI_RESUME");
				mUtilBoxStrings[UI_BOX_PAUSE_INDEX] = UI_BOX_PAUSE_STRING;
			}
			return 3;
		}
		else if (idx == UI_BOX_UTIL_INDEX_OFFSET + UI_BOX_FINISH_INDEX){
			//seleve.SetActionName("UI_FINISH");
			if (this->IsAnswerSet()){
				mFinished = true;
				//seleve.SetActionValue("SUCCESS");
				return 2;
			}
			else{
				//seleve.SetActionValue("FAILED");
				return 1;
			}
		}
		else if (idx >= UI_BOX_ANSWER_INDEX_OFFSET){
			mFinished = false;
			mUserSelection = idx;
			clearBoxStatus(UI_BOX_SELECTED);
			mAnswerBoxStatus[idx - UI_BOX_ANSWER_INDEX_OFFSET] = 
				upgradeStatus(mAnswerBoxStatus[idx - UI_BOX_ANSWER_INDEX_OFFSET], UI_BOX_SELECTED);
			mUtilBoxStatus[UI_BOX_FINISH_INDEX] = UI_BOX_READY;
			//seleve.SetActionName("UI_SELECTION");
			//seleve.SetActionValue(mAnswerBoxStrings[idx - UI_BOX_ANSWER_INDEX_OFFSET]);
			return 1;
		}
	}
	return 0;
}
