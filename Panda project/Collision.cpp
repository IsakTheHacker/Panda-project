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
#include "CollisionHandlerQueue.h"
#include "collisionSphere.h"
#include "collisionRay.h"

#include <geoMipTerrain.h>

#include <perlinNoise2.h>
#include <perlinNoise3.h>
#include <texturePool.h>
#include <cardMaker.h>

// Global stuff
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
PT(TextNode) text = new TextNode("textnode");
NodePath camera;


int main() {
	PandaFramework framework;
	framework.open_framework();
	framework.set_window_title("asd");

	// Open the window
	WindowFramework* window = framework.open_window();
	camera = window->get_camera_group();

	CollisionTraverser* collTrav = new CollisionTraverser();
	CollisionHandlerPusher pusher;

	// Load a model.
	NodePath smiley = window->load_model(framework.get_models(), "smiley");
	// Reparent the model to the camera so we can move it.
	smiley.reparent_to(camera);
	// Set the initial position of the model in the scene.
	smiley.set_pos(0, 25.5, 0.5)
	// Create a collision node for this object.
	cNode = CollisionNode('smiley')
	// Attach a collision sphere solid to the collision node.
	cNode.addSolid(CollisionSphere(0, 0, 0, 1.1))
	// Attach the collision node to the object's model.
	smileyC = smiley.attachNewNode(cNode)
	// Set the object's collision node to render as visible.
	smileyC.show()
	// Load another model.
	frowney = loader.loadModel('frowney')
	// Reparent the model to render.
	frowney.reparentTo(render)
	// Set the position of the model in the scene.
	frowney.setPos(5, 25, 0)

		// Create a collsion node for this object.
		cNode = CollisionNode('frowney')
		// Attach a collision sphere solid to the collision node.
		cNode.addSolid(CollisionSphere(0, 0, 0, 1.1))
		// Attach the collision node to the object's model.
		frowneyC = frowney.attachNewNode(cNode)
		// Set the object's collision node to render as visible.
		frowneyC.show()

		// Add the Pusher collision handler to the collision traverser.
		base.cTrav.addCollider(frowneyC, pusher)
		// Add the 'frowney' collision node to the Pusher collision handler.
		pusher.addCollider(frowneyC, frowney, base.drive.node())

		// Have the 'smiley' sphere moving to help show what is happening.
		frowney.posInterval(5, Point3(5, 25, 0), startPos = Point3(-5, 25, 0), fluid = 1).loop()

		// Run the scene.Move around with the mouse to see how the moving sphere changes
		// course to avoid the one attached to the camera.
		run()

	return 0;
}

