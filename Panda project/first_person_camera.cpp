#include "pandabase.h"
#include "pandaFramework.h"
#include "pandaSystem.h"
#include "GenericAsyncTask.h"
#include "AsyncTaskManager.h"
#define CAM_SPEED_ROT 0.05
#define CAM_SPEED_MOV 0.2
#define DEGTORAD(x) x * 0.01745329252
enum CAMERA_KEY {
	CKEY_W = 0,
	CKEY_A = 1,
	CKEY_S = 2,
	CKEY_D = 3
};
PandaFramework framework;
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
float alpha = 0.0f, beta = 0.0f;
bool keys_pressed[4] = { false, false, false, false };
void KeyEvent(const Event* e, void* data)
{
	//Key pressed or released?
	if (e->get_name() == "w")
		keys_pressed[CKEY_W] = true;
	else if (e->get_name() == "w-up")
		keys_pressed[CKEY_W] = false;
	else if (e->get_name() == "s")
		keys_pressed[CKEY_S] = true;
	else if (e->get_name() == "s-up")
		keys_pressed[CKEY_S] = false;
	else if (e->get_name() == "a")
		keys_pressed[CKEY_A] = true;
	else if (e->get_name() == "a-up")
		keys_pressed[CKEY_A] = false;
	else if (e->get_name() == "d")
		keys_pressed[CKEY_D] = true;
	else if (e->get_name() == "d-up")
		keys_pressed[CKEY_D] = false;
}
AsyncTask::DoneStatus CamRotationTask(GenericAsyncTask* task, void* data)
{
	int cursorX, cursorY;
	WindowFramework* win = framework.get_window(0);
	NodePath cam = win->get_camera_group();
	//Get cursor position
	cursorX = win->get_graphics_window()->get_pointer(0).get_x();
	cursorY = win->get_graphics_window()->get_pointer(0).get_y();
	//Subtract half the window size to retrieve
	//the relative cursor movement
	cursorX -= (int)(win->get_graphics_window()->get_x_size() / 2);
	cursorY -= (int)(win->get_graphics_window()->get_y_size() / 2);
	//Update the rotation angles
	alpha -= cursorX * CAM_SPEED_ROT;
	beta -= cursorY * CAM_SPEED_ROT;
	//Apply rotation to camera
	cam.set_hpr(alpha, beta, 0.0f);
	//Reset the cursor to its default position (middle of the window)
	win->get_graphics_window()->move_pointer(0,
		(int)(win->get_graphics_window()->get_x_size() / 2),
		(int)(win->get_graphics_window()->get_y_size() / 2));
	//Continue the task the next frame!
	return AsyncTask::DS_cont;
}
AsyncTask::DoneStatus CamMovementTask(GenericAsyncTask* task, void* data)
{
	//Move the camera
	NodePath cam = framework.get_window(0)->get_camera_group();
	float newX, newY, newZ;
	newX = cam.get_x();
	newY = cam.get_y();
	newZ = cam.get_z();
	if (keys_pressed[CKEY_W])
	{
		newX -= sin(DEGTORAD(alpha)) * CAM_SPEED_MOV;
		newY += cos(DEGTORAD(alpha)) * CAM_SPEED_MOV;
		newZ += sin(DEGTORAD(beta)) * CAM_SPEED_MOV;
	}
	else if (keys_pressed[CKEY_S])
	{
		newX += sin(DEGTORAD(alpha)) * CAM_SPEED_MOV;
		newY -= cos(DEGTORAD(alpha)) * CAM_SPEED_MOV;
		newZ -= sin(DEGTORAD(beta)) * CAM_SPEED_MOV;
	}
	if (keys_pressed[CKEY_A])
	{
		newX -= cos(DEGTORAD(alpha)) * CAM_SPEED_MOV;
		newY -= sin(DEGTORAD(alpha)) * CAM_SPEED_MOV;
	}
	else if (keys_pressed[CKEY_D])
	{
		newX += cos(DEGTORAD(alpha)) * CAM_SPEED_MOV;
		newY += sin(DEGTORAD(alpha)) * CAM_SPEED_MOV;
	}
	//Translate the camera
	cam.set_pos(newX, newY, newZ);
	return AsyncTask::DS_cont;
}
int main(int argc, char* argv[])
{
	WindowFramework* win;
	NodePath environ;
	//Open the framework & create a window
	framework.open_framework(argc, argv);
	framework.set_window_title("Panda3D Tests");
	win = framework.open_window();
	//Enable the keyboard
	win->enable_keyboard();
	//Load the environment
	environ = win->load_model(framework.get_models(),
		"models/environment");
	environ.reparent_to(win->get_render());
	environ.set_scale(0.25f);
	environ.set_pos(-8, 42, 0);
	//Define keys
	framework.define_key("w", "", &KeyEvent, NULL);
	framework.define_key("s", "", &KeyEvent, NULL);
	framework.define_key("a", "", &KeyEvent, NULL);
	framework.define_key("d", "", &KeyEvent, NULL);
	framework.define_key("w-up", "", &KeyEvent, NULL);
	framework.define_key("s-up", "", &KeyEvent, NULL);
	framework.define_key("a-up", "", &KeyEvent, NULL);
	framework.define_key("d-up", "", &KeyEvent, NULL);
	//Add tasks to the task manager
	taskMgr->add(new GenericAsyncTask("Rotates the camera",
		&CamRotationTask, NULL));
	taskMgr->add(new GenericAsyncTask("Moves the camera",
		&CamMovementTask, NULL));
	//Enter the main loop
	framework.main_loop();
	//Close framework & quit
	framework.close_framework();
	return 0;
}