#pragma once
#include "myview.h"
class MyAntiAliasingView :
	public MyView
{
public:
	MyAntiAliasingView(void);
	~MyAntiAliasingView(void);
	
	virtual void Show();
};

