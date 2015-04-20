#pragma once
#include "myknot.h"
#include "MyTracts.h"

class MyTractsKnot :
	public MyKnot
{
public:
	MyTractsKnot(void);
	~MyTractsKnot(void);

	void SetBeta(float beta){ mBeta = beta; };
	void SetTracts(const MyTracts* tracts);
	void SetNumSegments(int n);

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

	void ComputeGeometry();
	void LoadBuffer();
	void LoadShader();

	float mapToSize(float s);
	float mBeta;

	const MyTracts* mTracts;
	int mFaces;

	int mDisplayList;
	MyArrayi mIdxOffset;

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

	int mTexUniform;
	int mNormalAttribute;
	int mPositionAttribute;
	int mTexCoordAttribute;
	int mRadiusAttribute;
	int mColorAttribute;
};

