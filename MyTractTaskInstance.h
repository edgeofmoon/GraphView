#pragma once

#include <ostream>

#include "MyKnot.h"
#include "MyTimer.h"
#include "MyString.h"
#include "MyArray.h"

class MyScene;
class MyView;
class MyTractsKnot;
class MyTractLegendKnot;
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

	void SetConfigFile(const MyString& fn);
	void SetParticipantIndex(int pid);
	void SetTotalTrialNumber(int tn);

	virtual void Build();
	virtual void Show();
	virtual void Destory();
	virtual bool SeeAny();
	virtual MyGenericNode* MakeSeeWhat();


	static void LogHeader(std::ostream& out);
	void Log(std::ostream& out);

	bool IsEmpty() const { return mIsEmpty; };
	void SetEmpty(bool bEmpty){ mIsEmpty = bEmpty; };

	int EventHandler(MyGenericEvent& eve);

protected:
	static MyString mDecimer;
	int mParticipantIndex;
	int mTrialIndex;
	int mTotalTrials;

	MyTimer mTimer;


	MyScene* mScene;
	MyView* mView;

	void updateViewProjection(int width, int height);
	MyString mConfigFileName;
	MyTractsKnot* mTractKnot;
	MyBoxKnot* mBoxKnot1;
	MyBoxKnot* mBoxKnot2;
	MyTractLegendKnot* mLegendKnot;
	float mBeta;
	MyProjection::MyProjectionEnum mProjectionMethod;

	MyTractTaskInterface* mInterface;

	// log
	int mUserAnswerIndex;
	int mCorrectAnswerIndex;

	int mShowMode;
	bool mIsEmpty;
};

