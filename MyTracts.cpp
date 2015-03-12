#include "MyTracts.h"
#include <algorithm>
#define LARGENUMBER 999999

MyTracts::MyTracts(void)
	:mBox(MyVec3f(LARGENUMBER,LARGENUMBER,LARGENUMBER), MyVec3f(-LARGENUMBER,-LARGENUMBER,-LARGENUMBER))
{
	mValueMin = LARGENUMBER;
	mValueMax = -LARGENUMBER;
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
	tracts.mValueMin = std::min(tracts.mValueMin, color.g);
	tracts.mValueMax = std::max(tracts.mValueMax, color.g);
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

const float MyTracts::GetValue(int i, int j) const{
	return mColors[i][j].g;
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

float MyTracts::GetMinValue() const{
	return mValueMin;
}

float MyTracts::GetMaxValue() const{
	return mValueMax;
}

float MyTracts::GetMinValue(const MyBoundingBox& box) const{
	float minValue = mValueMax;
	for (int i = 0; i < this->GetNumTracts(); i++){
		for (int j = 0; j < this->GetNumVertices(i); j++){
			if (box.IsIn(this->GetCoord(i, j))){
				float value = this->GetValue(i, j);
				minValue = std::min(minValue, value);
			}
		}
	}
	return minValue;
}

float MyTracts::GetMaxValue(const MyBoundingBox& box) const{
	float maxValue = mValueMin;
	for (int i = 0; i < this->GetNumTracts(); i++){
		for (int j = 0; j < this->GetNumVertices(i); j++){
			if (box.IsIn(this->GetCoord(i, j))){
				float value = this->GetValue(i, j);
				maxValue = std::max(maxValue, value);
			}
		}
	}
	return maxValue;
}


MyBoundingBox MyTracts::GetBoundingBox() const{
	return mBox;
}