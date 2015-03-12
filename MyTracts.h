#pragma once
#include "MyArray.h"
#include "MyColor4.h"
#include "MyBoundingBox.h"

class MyTracts
{
public:
	MyTracts(void);
	~MyTracts(void);

	friend MyTracts& operator<< (MyTracts& tracts, const MyVec3f& coord);
	friend MyTracts& operator<< (MyTracts& tracts, const MyColor4f& color);

	void NewTract(int size = -1);

	const MyVec3f& GetCoord(int i, int j) const;
	const MyColor4f& GetColor(int i, int j) const;
	const float GetValue(int i, int j) const;
	int GetNumTracts() const;
	int GetNumVertices(int i) const;
	int GetTotalNumVertices() const;
	
	float GetMinValue() const;
	float GetMaxValue() const;
	float GetMinValue(const MyBoundingBox& box) const;
	float GetMaxValue(const MyBoundingBox& box) const;

	MyBoundingBox GetBoundingBox() const;

protected:
	MyArray<MyArray3f> mCoords;
	MyArray<MyArray<MyColor4f>> mColors;
	MyBoundingBox mBox;
	float mValueMin, mValueMax;
};

