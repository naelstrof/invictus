// scene.hpp: System that handles containing, organization, and searching of in-game objects.

#ifndef IS_SCENE_H_
#define IS_SCENE_H_

#include <vector>

#include "node.hpp"

namespace is {

class Scene {
    std::vector<is::Node*> m_nodes;
public:
    ~Scene();
    void            tick( float dt );
    void            draw();
};

};

extern is::Scene* world;
extern is::Scene* gui;

#endif // IS_SCENE_H_
