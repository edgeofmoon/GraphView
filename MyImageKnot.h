#pragma once

#include "myknot.h"
#include "MyImage.h"

class MyImageKnot :
	public MyKnot
{
public:
	MyImageKnot(void);
	~MyImageKnot(void);

	void SetImage(const MyImage* image);
	void SetSize(int width, int height);

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

	int mDisplayList;
	int mTexture;

	const MyImage* mImage;
	int mWidth, mHeight;
};

