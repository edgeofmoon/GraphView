#pragma once
#include "MyKnot.h"
#include "MyBoundingBox.h"

class MyBoxKnot :
	public MyKnot
{
public:
	MyBoxKnot();
	~MyBoxKnot();

	void SetBox(const MyBoundingBox& box);
	const MyBoundingBox& GetBox() const;

	virtual void Build();
	virtual void Show();
	virtual void Destory();
	virtual bool SeeAny();
	virtual MyGenericNode* MakeSeeWhat();

protected:
	virtual int mousePressEventHandler(MyGenericEvent& eve);
	virtual int mouseReleaseEventHandler(MyGenericEvent& eve);
	virtual int mouseMoveEventHandler(MyGenericEvent& eve);
	virtual int idleEventHandler(MyGenericEvent& eve);
	virtual int windowResizeEventHandler(MyGenericEvent& eve);

	virtual void responseBroadcast(const MyKnot* source, int type);

	MyBoundingBox mBox;

	int mDisplayList;
};

