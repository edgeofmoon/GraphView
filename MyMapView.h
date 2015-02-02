#pragma once
#include "myview.h"

class MyMapView :
	public MyView
{
public:
	MyMapView(void);
	~MyMapView(void);

	virtual void Build();
	virtual void Show();
	virtual void SetView(MyView* view);

	//virtual int EventHandler(MyGenericEvent& eve);
	
protected:
	
	virtual int mousePressEventHandler(MyGenericEvent& eve);
	virtual int mouseReleaseEventHandler(MyGenericEvent& eve);
	virtual int mouseMoveEventHandler(MyGenericEvent& eve);
	virtual int windowResizeEventHandler(MyGenericEvent& eve);

	MyView* mView;
};

