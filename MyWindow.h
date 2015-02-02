#pragma once
#include "mygenericeventlistener.h"

#include "MyView.h"
#include "MyArray.h"

class MyWindow :
	public MyGenericEventListener
{
public:
	MyWindow(void);
	~MyWindow(void);
	
	virtual void Show();
	void AddView(MyView* view);
	
	virtual int EventHandler(MyGenericEvent& eve);

protected:
	
	virtual int windowResizeEventHandler(MyGenericEvent& eve);

	MyArray<MyView*> mViews;

	int mWidth;
	int mHeight;
	float mMapHeightRatio;

	MyView* mFocusView;
};

