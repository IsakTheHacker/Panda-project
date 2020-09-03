//Panda3d libraries
#include "pgButton.h"
#include "pandaFramework.h"
#include "pandaSystem.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"
#include <mouseWatcher.h>

#include "collisionTraverser.h"
#include "collisionHandlerPusher.h"
#include "collisionNode.h"
#include "collisionHandlerFluidPusher.h"
#include "collisionSphere.h"
#include "collisionRay.h"
#include "collisionHandlerQueue.h"

PandaFramework framework;
NodePath camera;
PT(MouseWatcher) mouseWatcher;
PT(CollisionRay) pickerRay;
WindowFramework* window;
CollisionTraverser myTraverser = CollisionTraverser("ctraverser");
PT(CollisionHandlerQueue) myHandler;
PT(CollisionNode) pickerNode;
PT(CollisionHandlerQueue) handlePickedObject;
NodePath pickerNP;
NodePath pickedObj;




void myFunction() {



    pickerRay->set_from_lens(window->get_camera(0), window->get_graphics_window()->get_pointer(0).get_x()
        , window->get_graphics_window()->get_pointer(0).get_y());
    myTraverser.traverse(window->get_render());

    if (myHandler->get_num_entries() > 0)
    {
        myHandler->sort_entries();
        pickedObj = myHandler->get_entry(0)->get_into_node_path();
        pickedObj = pickedObj.find_net_tag("myObjectTag");


    }




}

void mousee(const Event* theEvent, void* data)
{
    myFunction();


}

int main(int argc, char** argv) {
    // Open a new window framework and set the title
    framework.open_framework(argc, argv);
    framework.set_window_title("My Panda3D Window");

    // Open the window
    window = framework.open_window();
    camera = window->get_camera_group(); // Get the camera and store it
    window->enable_keyboard();
    window->setup_trackball();
    pickerNode = new CollisionNode("mouseRay");
    pickerNP = camera.attach_new_node(pickerNode);
    pickerNode->set_from_collide_mask(GeomNode::get_default_collide_mask());
    pickerRay = new CollisionRay();
    pickerNode->add_solid(pickerRay);
    myHandler = new CollisionHandlerQueue();
    myTraverser.add_collider(pickerNP, myHandler);
    framework.define_key("mouse1", "click", mousee, 0);

    // Load the environment model
    NodePath environment = window->load_model(framework.get_models(), "models/environment");
    environment.reparent_to(window->get_render());
    environment.set_scale(0.25, 0.25, 0.25);
    environment.set_pos(-8, 42, 0);

    // Load our panda
    NodePath pandaActor = window->load_model(framework.get_models(), "panda-model");
    pandaActor.set_scale(0.005);
    pandaActor.reparent_to(window->get_render());
    pandaActor.set_tag("myObjectTag", "1");




    // Load the walk animation
    window->load_model(pandaActor, "panda-walk4");
    window->loop_animations(0);



    Thread* current_thread = Thread::get_current_thread();
    while (framework.do_frame(current_thread))
    {


    }
    framework.close_framework();
    return (0);
}