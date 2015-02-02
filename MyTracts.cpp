#include "MyTracts.h"
#define LARGENUMBER 999999

MyTracts::MyTracts(void)
	:mBox(MyVec3f(LARGENUMBER,LARGENUMBER,LARGENUMBER), MyVec3f(-LARGENUMBER,-LARGENUMBER,-LARGENUMBER))
{
}


MyTracts::~MyTracts(void)
{
}

MyTracts& operator<< (MyTracts& tracts, const MyVec3f& coord){
	tracts.mCoords.back() << coord;
	tracts.mBox.Engulf(coord);
	return tracts;
}

MyTracts& operator<< (MyTracts& tracts, const MyColor4f& color){
	tracts.mColors.back() << color;
	return tracts;
}

void MyTracts::NewTract(int size){
	MyArray3f coords;
	MyArray<MyColor4f> colors;
	if(size >= 0){
		coords.reserve(size);
		colors.reserve(size);
	}
	mCoords << coords;
	mColors << colors;
}

const MyVec3f& MyTracts::GetCoord(int i, int j) const{
	return mCoords[i][j];
}

const MyColor4f& MyTracts::GetColor(int i, int j) const{
	return mColors[i][j];
}

int MyTracts::GetNumTracts() const{
	return mCoords.size();
}

int MyTracts::GetNumVertices(int i) const{
	return mCoords[i].size();
}

int MyTracts::GetTotalNumVertices() const{
	int n = 0;
	for(int i = 0;i<this->GetNumTracts();i++){
		n += this->GetNumVertices(i);
	}
	return n;
}

MyBoundingBox MyTracts::GetBoundingBox() const{
	return mBox;
}