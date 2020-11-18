#include "pandaSystem.h"
#include "pandaFramework.h"
#include "windowProperties.h"
#include "TexturePool.h"
#include "mouseWatcher.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "collisionRay.h"
#include "collisionSphere.h"


PT(CollisionRay) pickerRay;
PT(CollisionNode) pickerNode;
NodePath pickerNP;
PT(CollisionRay)m_pickerRay;


Camera* camera2;


NodePath camera;

PT(CollisionHandlerQueue) m_cHandlerPtr;
CollisionTraverser c_trav;
PT(MouseWatcher) mouseWatcher;


int x = -8, y = 0;
bool mozogalepke = false;
NodePath lepecs;
NodePath iro;
PandaFramework framework;
WindowFramework* window;
TexturePool* myTexturePool = TexturePool::get_global_ptr();
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();

PT(GenericAsyncTask) task;
PT(Texture) texture;

PT(CollisionHandlerQueue) queue;
NodePath cameraCollisionNodePath2;

void fogd(const Event* event, void* data) {


	mouseWatcher = DCAST(MouseWatcher, window->get_mouse().node());
	PT(MouseWatcher) mouseWatcher =
		DCAST(MouseWatcher, window->get_mouse().node());
	if (mouseWatcher->has_mouse())
	{


		// get the mouse position
		 //const LPoint2f& mpos = mouseWatcher->get_mouse();
		camera2 = DCAST(Camera, window->get_camera_group().node()->get_child(0));
		// Set the position of the ray based on the mouse position
		m_pickerRay->set_from_lens(camera2, mouseWatcher->get_mouse_x(), mouseWatcher->get_mouse_y());
		//m_pickerRay->set_from_lens(window->get_camera(0), mpos);

	}

	// NodePath renderNp = window->get_render();
	  //		c_trav.traverse(renderNp);
   //queue->sort_entries();
  // if(queue->get_num_entries() > 0 &&
	//    queue->get_entry(0)->get_from_node()->get_name() == "lepke")
	  //  {
		//Message message;
	//}
 // LPoint2 mpos = mouseWatcher->get_mouse();
		  //if (window->get_graphics_window()){
		  //int x = window->get_graphics_window()->get_pointer(0).get_x();
  //int y = window->get_graphics_window()->get_pointer(0).get_y();

   //m_pickerRay->set_from_lens(window->get_camera(0), x, y);
  //		}
  //Message message;
  //myMouseWatcher = MouseWatcher()
  //mouseWatcher.getParent().attachNewNode(myMouseWatcher) 
  //mouseWatcher =DCAST(MouseWatcher, window->get_mouse().node());
  //if (mouseWatcher->has_mouse()){
	// The mouse is probably outside the screen.
	//return;
 //}
  //message.Messagewindow("yay");
 // This gives up the screen coordinates of the mouse.
// LPoint2f mpos = mouseWatcher->get_mouse();

 // This makes the ray's origin the camera and makes the ray point 
 // to the screen coordinates of the mouse.
 //pickerRay->set_from_lens(window->get_camera(0), mpos.get_x(), mpos.get_y());

//Dataeditor d;
//message.Messagewindow(d.Tostring(mpos.get_x()));
   //NodePath renderNp = window->get_render();
	//		c_trav.traverse(renderNp);

  //queue->sort_entries();
 //if(queue->get_num_entries() > 0 &&
   //   queue->get_entry(0)->get_from_node()->get_name() == "lepke")
	 // {
	mozogalepke = true;
	// }
}
void ereszd(const Event* event, void* data) {

	mozogalepke = false;
}
AsyncTask::DoneStatus  example_task(GenericAsyncTask* task, void* data) {
	if (window->get_graphics_window()) {

		x = window->get_graphics_window()->get_pointer(0).get_x() - 320;
		y = window->get_graphics_window()->get_pointer(0).get_y() - 240;
		if (mozogalepke) lepecs.set_pos((x / 5), 200, ((-1 * y) / 5));
		NodePath renderNp = window->get_render();
		c_trav.traverse(renderNp);

		queue->sort_entries();
		if (queue->get_num_entries() > 0 &&
			queue->get_entry(0)->get_from_node()->get_name() == "lepke")
		{
		}


	}




	return AsyncTask::DS_cont;
}






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {

	int argc = 0;
	char** argv = NULL;

	WindowProperties* props = new WindowProperties();
	props->set_size(640, 480);

	props->set_origin(100, 100);
	props->set_title("yay");


	//.set_window_title("yay");
	framework.open_framework(argc, argv);




	window = framework.open_window(*props, 1);

	PT(MouseWatcherRegion) r = new MouseWatcherRegion("region", 0, 640, 480, 0);

	//MouseWatcher::init_type();
	//r->set_active(true);

	//mouseWatcher->add_region(r);



	lepecs = window->load_model(framework.get_models(), "lepke.egg");
	lepecs.set_pos(-8, 200, 0);
	//lepecs.set_hpr(0, 0, 30);
	lepecs.set_scale(1, 1, 1);
	//lepecs.set_color(0,0,1,0.5);
	//myTexture = loader.loadTexture("myTexture.png")

	texture = TexturePool::load_texture("butterfly.jpg");


	lepecs.set_texture(texture);
	lepecs.reparent_to(window->get_render());


	//----------------------------------------
	iro = window->load_model(framework.get_models(), "model.egg");
	iro.set_pos(30, 200, 0);
	//lepecs.set_hpr(0, 0, 30);
	iro.set_scale(1, 1, 1);
	//lepecs.set_color(0,0,1,0.5);
	//myTexture = loader.loadTexture("myTexture.png")

	texture = TexturePool::load_texture("textura.png");


	iro.set_texture(texture);
	iro.reparent_to(window->get_render());
	//----------------------------------------


	task = new GenericAsyncTask("MyTaskName", &example_task, (void*)NULL);
	AsyncTaskManager::get_global_ptr()->add(task);

	PandaFramework* pf = window->get_panda_framework();
	pf->define_key("mouse1", "mouse1Downx", fogd, (void*)NULL);
	pf->define_key("mouse1-up", "mouse1Downx", ereszd, (void*)NULL);

	window->enable_keyboard();








	camera = window->get_camera_group();
	camera2 = DCAST(Camera, window->get_camera_group().node()->get_child(0));
	//-------------------------------------------------------------
	//mainCamera = window->get_camera_group();
	  // mainCamera.set_pos(0, -50, 2);
	PT(CollisionSphere) cs = new CollisionSphere(0, 0, 0, 10);
	PT(CollisionNode) cSphere_node = new CollisionNode("lepke");

	//cSphere_node-> show();
	cSphere_node->add_solid(cs);
	NodePath cameraCollisionNodePath = lepecs.attach_new_node(cSphere_node);
	cameraCollisionNodePath.show();
	//--------------------------------------------------------------

	PT(CollisionSphere) cs2 = new CollisionSphere(0, 0, 0, 20);
	PT(CollisionNode) cSphere_node2 = new CollisionNode("iro");

	//cSphere_node-> show();
	cSphere_node2->add_solid(cs2);
	cameraCollisionNodePath2 = iro.attach_new_node(cSphere_node2);
	cameraCollisionNodePath2.show();
	//-----------------------------------------------------------------

	queue = new CollisionHandlerQueue();
	if (queue != NULL)
	{
		c_trav.add_collider(cameraCollisionNodePath2, queue);
		//c_trav.traverse(get_render());



		c_trav.add_collider(cameraCollisionNodePath, queue);

		//c_trav.traverse(get_render());
	 //message.Messagewindow("yay");
	}
	//message.Messagewindow(d.Tostring( queue->get_num_entries()));
//-----------------------------------------mouseclicktester
	pickerRay = new CollisionRay();
	pickerNode = new CollisionNode("mouseRay");
	pickerNode->add_solid(pickerRay);
	pickerNode->set_from_collide_mask(GeomNode::get_default_collide_mask());

	pickerNP = window->get_camera_group();
	pickerNP.attach_new_node(pickerNode);

	pickerNP.show();
	c_trav.add_collider(pickerNP, queue);

	//----------------------------------------------------------
	framework.main_loop();
	framework.close_framework();
}