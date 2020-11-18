#include "pandaFramework.h"
#include "pandaSystem.h"
#include <collisionNode.h>
#include <collisionRay.h>
#include <collisionTraverser.h>
#include <collisionHandlerQueue.h>
#include "load_prc_file.h"
#include <geoMipTerrain.h>  


PandaFramework framework;
CollisionTraverser trav;
CollisionHandlerQueue* queue;
CollisionRay* ray;


void clickOnObject(const Event* ev, void* data) {
    WindowFramework* win = framework.get_window(0);
    float x = (2 * win->get_graphics_window()->get_pointer(0).get_x()) / ((float)win->get_graphics_window()->get_x_size()) - 1;
    float y = (2 * win->get_graphics_window()->get_pointer(0).get_y()) / ((float)win->get_graphics_window()->get_y_size()) - 1;
    y *= -1;
    ray->set_from_lens(framework.get_window(0)->get_camera(0), x, y);
    trav.traverse(framework.get_window(0)->get_render());
    std::cout << queue->get_num_entries() << std::endl;
}

int main(int argc, char* argv[]) {
    framework.open_framework(argc, argv);
    framework.set_window_title("My Panda3D Window");
    WindowFramework* window = framework.open_window();
    framework.get_window(0)->enable_keyboard();

    load_prc_file_data("", "notify-level-collide debug");

    // terrain
    GeoMipTerrain* terrain;
    terrain = new GeoMipTerrain("mySimpleTerrain");
    terrain->set_heightfield(Filename("map.png"));
    terrain->get_root().reparent_to(window->get_render());
    terrain->generate();
    terrain->get_root().set_collide_mask(BitMask32::bit(22));
    // look at the terrain
    window->get_camera_group().set_pos(0, 0, 20);
    window->get_camera_group().look_at(terrain->get_root());
    // collision system
    queue = new CollisionHandlerQueue();
    CollisionNode* cnode = new CollisionNode("ray");
    ray = new CollisionRay();
    cnode->add_solid(ray);
    cnode->set_from_collide_mask(BitMask32::bit(22));
    NodePath cpath(cnode);
    cpath.reparent_to(window->get_camera_group());
    trav.add_collider(cpath, queue);

    framework.define_key("mouse1", "click", clickOnObject, NULL);

    framework.main_loop();
    framework.close_framework();
    return (0);
}