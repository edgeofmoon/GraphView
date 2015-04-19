#pragma once

#include "MyGenericEventListener.h"
#include "MyArray.h"
#include "MyBox.h"

class MyTractTaskInterface :
	public MyGenericEventListener
{
public:
	MyTractTaskInterface();
	~MyTractTaskInterface();

	void Show();
	void Build();

	void SetEnable(bool b){ mEnabled = b; };
	void SetIndex(int i){ mIdx = i; };
	void SetTotalNumTrials(int total){ mTotal = total; };
	void SetResultShown(bool b = true) { mShowResult = b; };
	void SetWarningText(const MyString& str){ mWarningString = str; };
	void SetUserSelection(int idx){ mUserSelection = idx; };
	void SetEmpty(bool bemp){ mEmpty = bemp; };

	int GetUserSelection() const{ return mUserSelection; };
	bool IsPaused() const{ return mIsPaused; }

	bool IsAnswerSet() const;
	int GetHitBoxIndex(int mx, int my, int winWidth, int winHeight) const;

	int EventHandler(MyGenericEvent& eve);

protected:
	bool mEnabled;

	int mIdx;
	int mTotal;
	bool mShowResult;

	MyString mWarningString;

	virtual int mouseReleaseEventHandler(MyGenericEvent& eve);
	virtual int mouseMoveEventHandler(MyGenericEvent& eve);
	virtual int mousePressEventHandler(MyGenericEvent& eve);

	int mUserSelection;

	bool mEmpty;
	bool mFinished;

	bool mIsPaused;

	void initBoxes(bool bEmpty);
	int	upgradeStatus(int status0, int status1);
	int clearBoxStatus(int status);
	bool acceptClick(int status) const;
	bool acceptHover(int status) const;
	void drawBox(const MyBox2f& box, const MyString& str, const int status);
	MyArray<MyBox2f> mAnswerBoxes;
	MyArrayStr mAnswerBoxStrings;
	MyArrayi mAnswerBoxStatus;

	MyArray<MyBox2f> mUtilBoxes;
	MyArrayStr mUtilBoxStrings;
	MyArrayi mUtilBoxStatus;
};

