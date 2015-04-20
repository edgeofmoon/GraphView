#include "MyString.h"
#include "MyArray.h"
#include "MyDataLoader.h"
#include "MyGraphicsTool.h"
#include "MyGraphKnot.h"
#include "MyView.h"
#include "MyAntiAliasingView.h"
#include "MyWindow.h"
#include "MyRenderScene.h"
#include "MyUtility.h"

#include "MyTractsKnot.h"
#include "MyBoxKnot.h"
#include "MyTractTaskInterface.h"
#include "MyTractTaskInstance.h"

#include <GL/freeglut.h>
#include <iostream>
#include <sstream>
using namespace std;


MyGraphicsTool helper;
MyTractTaskInstance* instance = 0;

int pid = 0;
int currentIdx = 0;
MyArrayi configSeq;

// suppose only one keyboard or mouse
MyGenericEvent::MyMouseKey lastEventMouseKey = MyGenericEvent::MOUSE_KEY_LEFT;

void nextTask(){
	delete instance;
	currentIdx++;
	if (currentIdx >= configSeq.size()){
		exit(1);
	}
	else if (currentIdx == configSeq.size()){
		instance = new MyTractTaskInstance(pid, currentIdx, configSeq.size());
		instance->SetEmpty(true);
		instance->Build();
	}
	else{
		int configIdx = configSeq[currentIdx];
		instance = new MyTractTaskInstance(pid, currentIdx, configSeq.size());
		instance->SetConfigFile("configs\\config_" + MyString(configIdx) + ".txt");
		instance->Build();
	}
	helper.Update();
}

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
	instance->Show();
	helper.FreshScreen();
}
void ReshapeFunc(int w, int h){
	MyGenericEvent eve = MyGenericEvent::GenerateWindowResizeEvent(w, h);
	instance->EventHandler(eve);
	helper.FreshScreen();
}

void KeyReleaseFunc(unsigned char c, int x, int y){
	MyGenericEvent eve = MyGenericEvent::GenerateKeyboardKeyEvent(c, MyGenericEvent::KEY_UP, x, y, modiferState());
	helper.Update();
}
void KeyPressFunc(unsigned char c, int x, int y){
	MyGenericEvent eve = MyGenericEvent::GenerateKeyboardKeyEvent(c, MyGenericEvent::KEY_DOWN, x, y, modiferState());
	instance->EventHandler(eve);
	helper.Update();
}
void MouseKeyFunc(int button, int state, int x, int y){
	lastEventMouseKey = toKey(button);
	MyGenericEvent::MyKeyState keyState = toState(state);
	MyGenericEvent eve = MyGenericEvent::GenerateMouseKeyEvent(lastEventMouseKey, keyState, x, y, modiferState());
	int hrst = instance->EventHandler(eve);
	if (hrst == 2){
		nextTask();
	}
	helper.Update();
}

void MouseMoveFunc(int x, int y){
	MyGenericEvent eve = MyGenericEvent::GenerateMouseMoveEvent(
		lastEventMouseKey, MyGenericEvent::KEY_DOWN, x, y, modiferState());
	instance->EventHandler(eve);
	if (eve.DoNeedRedraw()){
		helper.Update();
	}
}

void MouseWheelFunc(int button, int dir, int x, int y){
	MyGenericEvent::MyKeyState keyState = toState(dir);
	MyGenericEvent eve = MyGenericEvent::GenerateMouseKeyEvent(MyGenericEvent::MOUSE_WHEEL, keyState, x, y, modiferState());
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

	configSeq = MyArrayi::GetSequence(0, 504 / 3 - 1);
	currentIdx = -1;
	nextTask();
	/*
	scene = new MyRenderScene;
	//view = new MyAntiAliasingView;
	view = new MyView;
	window = new MyWindow;
	MyTracts* tracts = MyDataLoader::MakeTractsFromFile("C:\\Users\\GuohaoZhang\\Dropbox\\data\\normal_s4_del0.data");
	MyTractsKnot *tractKnot = new MyTractsKnot;
	tractKnot->SetTracts(tracts);
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
	tractKnot->Build();
	tractKnot->SetName("normal_s5_boy");
	scene->AddKnot(tractKnot);
	view->SetScene(scene);
	scene->SetView(view);
	scene->Build();
	view->Build();
	ui.SetEnable(true);
	ui.Build();
	window->AddView(view);
	*/

	
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