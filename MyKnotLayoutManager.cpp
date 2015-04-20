#include "MyKnotLayoutManager.h"
#include "VisConfig.h"
#include "MyGlobalVariables.h"
#include <algorithm>
#include <cmath>

using namespace std;

MyKnotLayoutManager::MyKnotLayoutManager(){
	mKnotContainer = 0;
}

MyKnotLayoutManager::~MyKnotLayoutManager(void)
{
}

void MyKnotLayoutManager::Update(){
	this->initLayout();
}

void MyKnotLayoutManager::SetKnotContainer(const MyKnotContainer* knotContainer){
	mKnotContainer = knotContainer;
}

void MyKnotLayoutManager::initLayout(){
	int n = mKnotContainer->GetNumberKnots();
	int i = 0;
	float distance = OBJECT_DISTANCE;
	float offset = 0;
	float y = 0;

	/*
	for(vector<MyKnot*>::const_iterator
		itrKnot = mKnotContainer->Begin();
		itrKnot != mKnotContainer->End();
		itrKnot ++){
		MyVec3f center(0, y, -distance);
		//MyVec3f center(0, 0, -distance);
		(*itrKnot)->SetToDefault();
		//(*itrKnot)->Scale(3);
		(*itrKnot)->Translate(center);
		i++;
	}
	*/

	
	MyVec3f center(0, 0, -distance);
	vector<MyKnot*>::const_iterator itrKnot = mKnotContainer->Begin();
	(*itrKnot)->SetToDefault();
	(*itrKnot)->Scale(OBJECT_SCALE);
	(*itrKnot)->Translate(center);
	itrKnot++;
	(*itrKnot)->SetToDefault();
	(*itrKnot)->Scale(OBJECT_SCALE);
	(*itrKnot)->Translate(MyVec3f(120, 0, -distance));

}

const MyKnot* MyKnotLayoutManager::GetKnot(int i) const{
	return mKnotContainer->GetKnot(i);
}
int MyKnotLayoutManager::GetNumKnots() const{
	return mKnotContainer->GetNumberKnots();
}