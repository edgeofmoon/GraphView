#pragma once
#include "MyKnot.h"
#include "MyColor4.h"

class MyTractLegendKnot :
	public MyKnot
{
public:
	MyTractLegendKnot();
	~MyTractLegendKnot();

	void SetNumSegments(int n);
	void SetValueRange(const MyVec2f& valueRange);
	void SetScreenSpace(bool bs){ mbScreenSpace = bs; };
	void SetBeta(float beta){ mBeta = beta; };

	virtual void Build();
	virtual void Show();
	virtual void Destory();
	virtual bool SeeAny();
	virtual MyGenericNode* MakeSeeWhat();

protected:

	void ComputeGeometry();
	void LoadBuffer();
	void LoadShader();

	float mapToSize(float s);
	float mBeta;

	int mFaces;
	float mMinValue;
	float mMaxValue;
	MyVec3f mStartPoint;
	MyVec3f mEndPoint;

	int mDisplayList;

	MyArray3f mVertices;
	MyArray3f mNormals;
	MyArray2f mTexCoords;
	MyArrayf mRadius;
	MyArray<MyColor4f> mColors;
	MyArray3i mIndices;

	// for shader
	unsigned int mShaderProgram;
	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
	unsigned int mNormalBuffer;
	unsigned int mTexCoordBuffer;
	unsigned int mRadiusBuffer;
	unsigned int mColorBuffer;
	unsigned int mIndexBuffer;
	bool mbScreenSpace;

	int mTexUniform;
	int mNormalAttribute;
	int mPositionAttribute;
	int mTexCoordAttribute;
	int mRadiusAttribute;
	int mColorAttribute;
};

