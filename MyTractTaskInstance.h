#pragma once

#include <ostream>

#include "MyKnot.h"
#include "MyTimer.h"
#include "MyString.h"
#include "MyArray.h"

class MyScene;
class MyView;
class MyTractsKnot;
class MyBoxKnot;
class MyTractTaskInterface;

namespace MyProjection{
	enum MyProjectionEnum
	{
		INVALID = 0,
		ORTHOGONAL = 1,
		PERSPECTIVE = 2,
		SCREENSPACE = 3
	};
	MyString GetProjectionName(const MyProjectionEnum& projEnum);
	MyProjectionEnum GetProjectionEnum(const MyString& projStr);
}

class MyTractTaskInstance :
	public MyKnot
{
public:
	MyTractTaskInstance(int pid, int tid, int total);
	~MyTractTaskInstance();


	virtual void Build();
	virtual void Show();
	virtual void Destory();
	virtual bool SeeAny();
	virtual MyGenericNode* MakeSeeWhat();

	void SetConfigFile(const MyString& fn);
	void SetParticipantIndex(int pid);
	void SetTotalTrialNumber(int tn);

	static void LogHeader(std::ostream& out);
	void Log(std::ostream& out);

	bool IsEmpty() const { return mIsEmpty; };

	int EventHandler(MyGenericEvent& eve);

protected:
	int mParticipantIndex;
	int mTrialIndex;
	int mTotalTrials;

	MyTimer mTimer;

	MyScene* mScene;
	MyView* mView;

	MyString mConfigFileName;
	MyTractsKnot* mTractKnot;
	MyBoxKnot* mBoxKnot1;
	MyBoxKnot* mBoxKnot2;
	float mBeta;
	MyProjection::MyProjectionEnum mProjectionMethod;

	MyTractTaskInterface* mInterface;

	// log
	int mUserAnswerIndex;
	int mCorrectAnswerIndex;


	int mShowMode;
	bool mIsEmpty;
};

