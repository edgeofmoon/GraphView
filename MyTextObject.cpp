#include "MyTextObject.h"
#include "MyPrimitiveDrawer.h"

MyTextObject::MyTextObject(void)
{
	mTextTexture = -1;
}


MyTextObject::~MyTextObject(void)
{
	MyPrimitiveDrawer::DeleteTexture(mTextTexture);
	mTextTexture = -1;
}

void MyTextObject::Show(){

	MyPrimitiveDrawer::EnableTexture2D();
	MyPrimitiveDrawer::BindTexture2D(mTextTexture);
	MyPrimitiveDrawer::EnableAlplaBlending();
	MyPrimitiveDrawer::BeginQuads();

	float widthRatio = mWidth/(float)mTextureWidth;
	float heightRatio = mHeight/(float)mTextureHeight;

	MyPrimitiveDrawer::TextureCoordinate(MyVec2f(0,0));
	MyPrimitiveDrawer::Vertex(MyVec2f(0,0));
	
	MyPrimitiveDrawer::TextureCoordinate(MyVec2f(widthRatio,0));
	MyPrimitiveDrawer::Vertex(MyVec2f(mWidth,0));
	
	MyPrimitiveDrawer::TextureCoordinate(MyVec2f(widthRatio,heightRatio));
	MyPrimitiveDrawer::Vertex(MyVec2f(mWidth,mHeight));
	
	MyPrimitiveDrawer::TextureCoordinate(MyVec2f(0,heightRatio));
	MyPrimitiveDrawer::Vertex(MyVec2f(0,mHeight));

	MyPrimitiveDrawer::EndPrimitive();

	MyPrimitiveDrawer::DisableAlplaBlending();
	MyPrimitiveDrawer::UnbindTexture2D(mTextTexture);
	MyPrimitiveDrawer::DisableTexture2D();
	
	/*
	// draw the box
	MyPrimitiveDrawer::BeginLineLoop();
	MyPrimitiveDrawer::Vertex(MyVec2f(0,0));
	MyPrimitiveDrawer::Vertex(MyVec2f(mWidth,0));
	MyPrimitiveDrawer::Vertex(MyVec2f(mWidth,mHeight));
	MyPrimitiveDrawer::Vertex(MyVec2f(0,mHeight));
	MyPrimitiveDrawer::EndPrimitive();
	*/
	
}

void MyTextObject::SetTextureIndex(int tex){
	mTextTexture = tex;
}

int MyTextObject::GetTextTextureIndex() const{
	return mTextTexture;
}

void MyTextObject::Build(){
	if(!MyPrimitiveDrawer::IsTexture(mTextTexture)){
		mTextTexture = MyPrimitiveDrawer::GenerateTexture();
	}
	
	int pixelBoarder = 0;
	mWidth = MyPrimitiveDrawer::GetBitMapTextLargeWidth(mString)+2*pixelBoarder;
	mHeight = MyPrimitiveDrawer::GetBitmapLargeHeight()+2*pixelBoarder;
	
	mTextureWidth = 1;
	while(mTextureWidth < mWidth) mTextureWidth*=2;
	mTextureHeight = 1;
	while(mTextureHeight < mHeight) mTextureHeight*=2;


	MyPrimitiveDrawer::EnableTexture2D();
	MyColor4f clearColor = MyPrimitiveDrawer::GetClearColor();
	MyPrimitiveDrawer::SetClearColor(MyColor4f(0,0,0,0));
	MyPrimitiveDrawer::ClearFrameBuffer();
	MyPrimitiveDrawer::PushMatrix();
	MyPrimitiveDrawer::PushProjectionMatrix();
	MyVec4i viewport = MyPrimitiveDrawer::GetViewport();
	MyPrimitiveDrawer::SetViewport(MyVec4i(0,0, mTextureWidth, mTextureHeight));
	MyPrimitiveDrawer::SetToByPixelMatrix(mTextureWidth, mTextureHeight, 0, 1000);
	MyPrimitiveDrawer::Color(MyColor4f(1,1,1,1));
	MyPrimitiveDrawer::DrawBitMapTextLarge(MyVec3f(pixelBoarder,pixelBoarder+mHeight/4,0), mString);
	MyPrimitiveDrawer::DrawBitMapTextLarge(MyVec3f(pixelBoarder,1+pixelBoarder+mHeight/4,0), mString);
	MyPrimitiveDrawer::SetViewport(viewport);
	MyPrimitiveDrawer::PopMatrix();
	MyPrimitiveDrawer::PopProjectionMatrix();

	MyPrimitiveDrawer::BindTexture2D(mTextTexture);
	MyPrimitiveDrawer::SpecifyTextureImage2D(mTextureWidth, mTextureHeight);
	MyPrimitiveDrawer::AutoSpecifyTexutreParamters();
	MyPrimitiveDrawer::CopyToTextureImage2D(0,0,mTextureWidth, mTextureHeight);
	MyPrimitiveDrawer::GenerateMipMap2D();
	MyPrimitiveDrawer::UnbindTexture2D(mTextTexture);
	MyPrimitiveDrawer::DisableTexture2D();
	
	MyPrimitiveDrawer::SetClearColor(clearColor);
	MyPrimitiveDrawer::ClearFrameBuffer();

}


void MyTextObject::SetString(const MyString& str){
	mString = str;
}
	
MyString MyTextObject::GetString() const{
	return mString;
}

int MyTextObject::GetWidth() const{
	return mWidth;
}

int MyTextObject::GetHeight() const{
	return mHeight;
}

int MyTextObject::GetTextureWidth() const{
	return mTextureWidth;
}

int MyTextObject::GetTextureHeight() const{
	return mTextureHeight;
}