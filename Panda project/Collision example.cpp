#include "pandaFramework.h"
#include "collisionHandlerPusher.h"
#include "collisionNode.h"
#include "collisionSphere.h"
#include "mouseWatcher.h"

int main(int argc, char* argv[])
{
	PandaFramework framework;
	// Open a new window framework and set the title
	framework.open_framework(argc, argv);
	framework.set_window_title("Pusher");
	framework.enable_default_keys();

	// Open a window
	WindowFramework* window = framework.open_window();
	// enable key events
	window->enable_keyboard();

	// Get a MouseWatcher from the created window
	// will be used to move nodes arround
	NodePath mouseNode = window->get_mouse();

	// Set up the scene
	NodePath camera = window->get_camera_group();
	camera.set_pos(0., 0., 35.);
	camera.set_p(-90.);

	NodePath smileyNode = window->load_model(window->get_render(), "smiley");
	smileyNode.set_pos(0., 0., 0.5);
	CollisionNode* cNode = new CollisionNode("smiley");
	cNode->add_solid(new CollisionSphere(0, 0, 0, 1.0));
	NodePath smileyC = smileyNode.attach_new_node(cNode);
	// smileyC.show();

	NodePath frowneyNode = window->load_model(window->get_render(), "frowney");
	frowneyNode.set_pos(5., 0., 0.5);
	cNode = new CollisionNode("frowney");
	cNode->add_solid(new CollisionSphere(0, 0, 0, 1.0));
	NodePath frowneyC = frowneyNode.attach_new_node(cNode);
	// frowneyC.show();

	// set up collisions handler and pusher
	CollisionHandlerPusher pusher;
	CollisionTraverser* collTrav = new CollisionTraverser();

	// Smiley will be pushed if a collision is detected
	pusher.add_collider(smileyC, smileyNode);
	collTrav->add_collider(smileyC, &pusher);

	// Try also this
	// collTrav->add_collider(frowneyC, &pusher);
	// pusher.add_collider(frowneyC, frowneyNode);

	// too lazy to create a task.
	Thread* current_thread = Thread::get_current_thread();
	while (framework.do_frame(current_thread))
	{
		if (!mouseNode.is_empty())
		{
			MouseWatcher* mouseWatcher =
				dynamic_cast<MouseWatcher*>(mouseNode.node());
			if (mouseWatcher != NULL && mouseWatcher->has_mouse())
			{
				frowneyNode.set_x(mouseWatcher->get_mouse_x() * 10);
				frowneyNode.set_y(mouseWatcher->get_mouse_y() * 10);
			}
		}
		// check collisions, will call pusher collision handler
		// if a collision is detected
		collTrav->traverse(window->get_render());
	}

	framework.close_framework();
	return 0;
}