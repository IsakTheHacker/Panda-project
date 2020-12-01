#include <iostream>
using namespace std;

#include "pandaFramework.h"
#include "pandaSystem.h"
#include "mouseButton.h"
#include "texturePool.h"

#include "actorNode.h"
#include "forceNode.h"
#include "physicsManager.h"
#include "linearEulerIntegrator.h"
#include "physical.h"
#include "forces.h"

#include "collisionTraverser.h"
#include "collisionHandlerFluidPusher.h"
#include "collisionSphere.h"
#include "collisionPlane.h"

#include "lvector3.h"
#include "plane.h"


PandaFramework framework;
WindowFramework* window;


class testObj : public Physical {
public:
	int doobie;

	testObj() : Physical() {

	}

	~testObj() {
	}
};


int main(int argc, char* argv[]) {
	//open a new window framework
	framework.open_framework(argc, argv);

	//set the window title to My Panda3D Window
	framework.set_window_title("My Panda3D Window");

	//open the window
	window = framework.open_window();

	//
	// This part sets up an object for physics, as in the physics tests.
	//

	// create a physics manager
	PhysicsManager pm;

	// to do anything, the physics manager needs an integrator. Here, one for linear forces.
	pm.attach_linear_integrator(new LinearEulerIntegrator);

	// create an actor node and add it to the scene graph under the render node.
	// the actor node already has a physical living inside it when we create it.
	NodePath annp = window->get_render().attach_new_node(new ActorNode("actor_node"));
	NodePath annp2 = window->get_render().attach_new_node(new ActorNode("actor_node2"));

	// register the node with the physics manager
	pm.attach_physical_node((PhysicalNode*)annp.node());
	pm.attach_physical_node((PhysicalNode*)annp2.node());

	// create a force		
	NodePath fnnp(new ForceNode("force_node"));
	((ForceNode*)fnnp.node())->add_force(new LinearVectorForce(0, 1.0, 0, 0.1));

	fnnp.reparent_to(annp);

	fnnp.output(cout); cout << endl;
	annp.output(cout); cout << endl;

	annp.set_pos(0, 10, 0);

	PT(ActorNode) an((ActorNode*)annp.node());
	an->get_physical(0)->add_linear_force((LinearForce*)((ForceNode*)fnnp.node())->get_force(0));



	//
	// this part prepares the collision system
	//

	// create a collision solid attached to our actor, in this case a sphere
	// around the actor.
	NodePath fromObj = annp.attach_new_node(new CollisionNode("an-col-node"));
	CollisionNode* n;
	n = (CollisionNode*)fromObj.node();
	n->add_solid(new CollisionSphere(0, 0, 0, 1));

	// create a collision solid that will form something to collide with.
	NodePath toObj = window->get_render().attach_new_node(new CollisionNode("p-col-node"));
	n = (CollisionNode*)toObj.node();
	n->add_solid(new CollisionPlane(LPlanef(LVector3f(0, -1, 0), LVector3f(0, 0, 0))));
	toObj.set_pos(0, 50, 0);

	// now we need a collision handler for the object
	CollisionHandlerFluidPusher* cha = new CollisionHandlerFluidPusher();
	cha->add_collider(fromObj, annp);


	// then we need a collision traverser
	CollisionTraverser traverser;
	traverser.add_collider(fromObj, cha);

	// and finally, we load some stuff to look at!
	NodePath m = window->load_model(framework.get_models(), "planet_sphere.egg.pz");
	PT(Texture) mt = TexturePool::load_texture("earth_1k_tex.jpg");
	m.reparent_to(annp);
	m.set_texture(mt);


	for (int cc = 0; cc < 100; ++cc)
	{
		// do physics
		pm.do_physics(1.0f);

		// do collisions
		traverser.traverse(window->get_render());

		// do events
		framework.get_task_mgr().poll();

		cout << annp.get_pos() << endl;
	}

	//close the window framework
	framework.close_framework();
	return (0);
}