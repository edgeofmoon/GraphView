
#include "MyString.h"

#include "MyArray.h"
#include "MyKnotLayoutManager.h"
#include "MyDataLoader.h"
#include "MyGraphicsTool.h"
#include "MyGraphKnot.h"
#include "MyView.h"
#include "MyAntiAliasingView.h"
#include "MyMapView.h"
#include "MyWindow.h"
#include "MyRenderScene.h"
#include "MyGenericEvent.h"
#include "MyGraphFilter.h"
#include "MyDenseGraph.h"
#include "MyUtility.h"

#include "MyGraphLayout.h"
#include "MyGraphMatrixLayout.h"
#include "MyGraphMatrixRepresentation.h"
#include "MyGraphIterativeLayout.h"
#include "MyGraphFruchtermanReingoldLayout.h"
#include "MyGraphLocalOptimizationLayout.h"
#include "MyGraphHierarchicalLayout.h"
#include "MyGraphLodLayout.h"
#include "MyGraphRadialLayout.h"
#include "MyGenericLayoutSyncer.h"

#include "MyTractsKnot.h"
#include "MyBitmap.h"
#include "MyImageKnot.h"
#include "MyBoxKnot.h"

#include "MyGraphEncodingRepresentation.h"
#include "MyGraphAlgorithm.h"
#include "MyGraphMatrixEncodingRepresentation.h"

#include "MyGraphEdgeBundler.h"
#include "MyGraphClusterEdgeBundler.h"
#include "MyGraphGeometryEdgeBundler.h"

#include <GL/freeglut.h>
#include <iostream>
#include <sstream>
using namespace std;


MyGraphicsTool helper;
MyView* view;
MyScene* scene;
MyWindow* window;

// suppose only one keyboard or mouse
MyGenericEvent::MyMouseKey lastEventMouseKey = MyGenericEvent::MOUSE_KEY_LEFT;

MyGenericEvent::MyMouseKey toKey(int k){
	return (k == 0 ?
		MyGenericEvent::MOUSE_KEY_LEFT : (k == 1 ?
		MyGenericEvent::MOUSE_KEY_MIDDLE : MyGenericEvent::MOUSE_KEY_RIGHT));
}

MyGenericEvent::MyKeyState toState(int s){
	return (s <= 0 ? MyGenericEvent::KEY_DOWN : MyGenericEvent::KEY_UP);
}

MyModifierState modiferState(){
	int modifers = helper.GetModifers();
	return MyModifierState(modifers);
}

void DisplayFunc(void){
	helper.ClearFrameBuffer();
	window->Show();
	helper.FreshScreen();
}
void ReshapeFunc(int w, int h){
	MyGenericEvent eve = MyGenericEvent::GenerateWindowResizeEvent(w, h);
	window->EventHandler(eve);
}


bool bugIgnore = false;

void KeyReleaseFunc(unsigned char c, int x, int y){
	MyGenericEvent eve = MyGenericEvent::GenerateKeyboardKeyEvent(c, MyGenericEvent::KEY_UP, x, y, modiferState());
	helper.Update();
}
void KeyPressFunc(unsigned char c, int x, int y){
	MyGenericEvent eve = MyGenericEvent::GenerateKeyboardKeyEvent(c, MyGenericEvent::KEY_DOWN, x, y, modiferState());
	window->EventHandler(eve);
	helper.Update();
}
void MouseKeyFunc(int button, int state, int x, int y){
	if (bugIgnore){
		return;
	}

	lastEventMouseKey = toKey(button);
	MyGenericEvent::MyKeyState keyState = toState(state);
	MyGenericEvent eve = MyGenericEvent::GenerateMouseKeyEvent(lastEventMouseKey, keyState, x, y, modiferState());
	window->EventHandler(eve);
	helper.Update();
}

void MouseMoveFunc(int x, int y){
	if (bugIgnore){
		return;
	}
	MyGenericEvent eve = MyGenericEvent::GenerateMouseMoveEvent(
		lastEventMouseKey, MyGenericEvent::KEY_DOWN, x, y, modiferState());
	window->EventHandler(eve);
	if (eve.DoNeedRedraw()){
		helper.Update();
	}
}

void MouseWheelFunc(int button, int dir, int x, int y){
	MyGenericEvent::MyKeyState keyState = toState(dir);
	MyGenericEvent eve = MyGenericEvent::GenerateMouseKeyEvent(MyGenericEvent::MOUSE_WHEEL, keyState, x, y, modiferState());
	window->EventHandler(eve);
	helper.Update();
}

void IdleFunc(){
	MyGenericEvent eve = MyGenericEvent::GenerateTimerEvent();
	//window->EventHandler(eve);
	if (eve.DoNeedRedraw()){
		helper.Update();
	}
}

int main(int argc, char* argv[]){
	
	helper.Init(&argc,argv);
	scene = new MyRenderScene;
	view = new MyAntiAliasingView;
	window = new MyWindow;

	MyTracts* tracts = MyDataLoader::MakeTractsFromFile("C:\\Users\\GuohaoZhang\\Dropbox\\data\\normal_s4_del0.data");
	MyTractsKnot *tractKnot = new MyTractsKnot;
	tractKnot->SetTracts(tracts);

	/*
	MyBoundingBox box1, box2;
	box1 = MyDataLoader::LoadBoundingBoxFromFile("C:\\Users\\GuohaoZhang\\Dropbox\\task_data\\data\\normal_allfb\\region_for_task1\\s3\\cc\\pos1\\tumorbox_0_region_s3.data");
	box2 = MyDataLoader::LoadBoundingBoxFromFile("C:\\Users\\GuohaoZhang\\Dropbox\\task_data\\data\\normal_allfb\\region_for_task1\\s3\\cc\\pos1\\tumorbox_1_region_s3.data");

	MyBoxKnot *boxknot1 = new MyBoxKnot;
	boxknot1->SetBox(box1);
	boxknot1->Build();
	//scene->AddKnot(boxknot1);

	MyBoxKnot *boxknot2 = new MyBoxKnot;
	boxknot2->SetBox(box2);
	boxknot2->Build();
	//scene->AddKnot(boxknot2);

	MyKnot::Connect(tractKnot, boxknot1);
	MyKnot::Connect(tractKnot, boxknot2);
	*/

	tractKnot->Build();
	tractKnot->SetName("normal_s5_boy");
	scene->AddKnot(tractKnot);

	view->SetScene(scene);
	scene->SetView(view);
	scene->Build();
	view->Build();

	window->AddView(view);

	helper.RegisterDisplayFunction(DisplayFunc);
	helper.RegisterReshapeFunction(ReshapeFunc);
	helper.RegisterKeyReleaseFunction(KeyReleaseFunc);
	helper.RegisterKeyPressFunction(KeyPressFunc);
	helper.RegisterMouseKeyFunction(MouseKeyFunc);
	helper.RegisterMouseMoveFunction(MouseMoveFunc);
	helper.RegisterIdleFunction(IdleFunc);
	helper.RegisterMouseWheelFunction(MouseWheelFunc);
	helper.Start();

	return 1;
}