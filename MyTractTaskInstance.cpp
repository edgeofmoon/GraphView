#include "MyTractTaskInstance.h"

#include "MyView.h"
#include "MyScene.h"
#include "MyTractsKnot.h"
#include "MyBoxKnot.h"
#include "MyTractTaskInterface.h"
#include "MyGraphicsTool.h"
#include "MyUtility.h"
#include "MyDataLoader.h"
#include "MyGlobalVariables.h"

#include <fstream>
#include <map>

using namespace std;
using namespace MyProjection;

MyString MyProjection::GetProjectionName(const MyProjectionEnum& projEnum){
	switch (projEnum){
		case ORTHOGONAL:
			return "ORTHOGONAL";
			break;
		case PERSPECTIVE:
			return "PERSPECTIVE";
			break;
		case SCREENSPACE:
			return "SCREENSPACE";
			break;
		default:
		case INVALID:
			return "INVALID";
			break;
	}
}

MyProjectionEnum MyProjection::GetProjectionEnum(const MyString& projStr){
	if (projStr == "ORTHOGONAL"){
		return ORTHOGONAL;
	}
	else if (projStr == "PERSPECTIVE"){
		return PERSPECTIVE;
	}
	else if (projStr == "SCREENSPACE"){
		return SCREENSPACE;
	}
	else return INVALID;
}

MyTractTaskInstance::MyTractTaskInstance(int pid, int tid, int total){
	mParticipantIndex = pid;
	mTrialIndex = tid;
	mTotalTrials = total;



	mIsEmpty = true;
}


MyTractTaskInstance::~MyTractTaskInstance(){
	this->Destory();
}

void MyTractTaskInstance::Build(){
	ifstream cfgFile(mConfigFileName);
	MyString line;
	if (!cfgFile.is_open()){
		cout << "Error: Cannot open config file: " << mConfigFileName << endl;
	}

	cfgFile >> line;
	MyTracts* tracts = MyDataLoader::MakeTractsFromFile(line);
	cfgFile >> line;
	MyBoundingBox box1 = MyDataLoader::LoadBoundingBoxFromFile(line);
	cfgFile >> line;
	MyBoundingBox box2 = MyDataLoader::LoadBoundingBoxFromFile(line);
	cfgFile >> mBeta;
	cfgFile >> line;
	mProjectionMethod = MyProjection::GetProjectionEnum(line);

	mTractKnot = new MyTractsKnot;
	mTractKnot->SetTracts(tracts);

	mBoxKnot1 = new MyBoxKnot;
	mBoxKnot1->SetBox(box1);
	mBoxKnot1->Build();
	//scene->AddKnot(boxknot1);

	mBoxKnot2 = new MyBoxKnot;
	mBoxKnot2->SetBox(box2);
	mBoxKnot2->Build();
	//scene->AddKnot(boxknot2);

	MyKnot::Connect(mTractKnot, mBoxKnot1);
	MyKnot::Connect(mTractKnot, mBoxKnot2);

	mTractKnot->SetBeta(mBeta);
	mTractKnot->Build();

	MyVec4i viewport = MyGraphicsTool::GetViewport();
	mTractKnot->GetTrackBall().Reshape(viewport[2], viewport[3]);

	mInterface = new MyTractTaskInterface;
	mInterface->SetEmpty(mIsEmpty);
	mInterface->Build();

	mView = new MyView;
	mScene = new MyScene;
	mScene->AddKnot(mTractKnot);
	mView->SetScene(mScene);
	mScene->SetView(mView);
	mScene->Build();
	mView->Build();
	mView->Build();

	updateViewProjection(1024, 768);

	mInterface = new MyTractTaskInterface;
	mInterface->SetEnable(true);
	mInterface->Build();
}

void MyTractTaskInstance::Show(){
	if (!mInterface->IsPaused()){
		mView->Show();
	}
	mInterface->Show();
}

void MyTractTaskInstance::Destory(){
	SafeFreeObject(mScene);
	SafeFreeObject(mView);
	// scene will destory tractknot
	//SafeFreeObject(mTractKnot);
	SafeFreeObject(mBoxKnot1);
	SafeFreeObject(mBoxKnot2);
	SafeFreeObject(mInterface);
}

bool MyTractTaskInstance::SeeAny(){
	if (mTractKnot){
		return mTractKnot->SeeAny();
	}
	return false;
}

MyGenericNode* MyTractTaskInstance::MakeSeeWhat(){
	if (mTractKnot){
		return mTractKnot->MakeSeeWhat();
	}
	return 0;
}

void MyTractTaskInstance::SetConfigFile(const MyString& fn){
	mConfigFileName = fn;
}

void MyTractTaskInstance::SetParticipantIndex(int pid){
	mParticipantIndex = pid;
}

void MyTractTaskInstance::SetTotalTrialNumber(int tn){
	mTotalTrials = tn;
}

void MyTractTaskInstance::LogHeader(std::ostream& out){
}

void MyTractTaskInstance::Log(std::ostream& out){
}


int MyTractTaskInstance::EventHandler(MyGenericEvent& eve){

	int UIHandleRst = mInterface->EventHandler(eve);

	if (UIHandleRst == 0){
		mView->SetViewport(MyVec4i(0,0,eve.GetWindowWidth(), eve.GetWindowHeight()));
		int viewHrst = mView->EventHandler(eve);
		if (eve.GetEventSource() == MyGenericEvent::WINDOW_RESIZE){
			updateViewProjection(eve.GetWindowWidth(), eve.GetWindowHeight());
		}
		return viewHrst;
	}

	// finished
	if (UIHandleRst == 2){
		// empty task does not do anything
		if (mIsEmpty) return UIHandleRst;
		mUserAnswerIndex = mInterface->GetUserSelection();
	}
	return UIHandleRst;
}

void MyTractTaskInstance::updateViewProjection(int width, int height){
	float aspect = width / (float)height;
	switch (mProjectionMethod)
	{
	case MyProjection::PERSPECTIVE:
	case MyProjection::SCREENSPACE:
		mView->SetProjectionMatrix(MyMatrixf::PerspectiveMatrix(
			PERSPECTIVE_FOV, aspect, VIEW_NEAR, VIEW_FAR));
		break;
	case MyProjection::INVALID:
	case MyProjection::ORTHOGONAL:
	default:
		mView->SetProjectionMatrix(MyMatrixf::OrthographicMatrix(
			VIEW_BOTTOM*aspect, VIEW_TOP*aspect, VIEW_BOTTOM, VIEW_TOP, VIEW_NEAR, VIEW_FAR));
		break;
	}
}