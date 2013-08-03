// scene.hpp: System that handles containing, organization, and searching of in-game objects.

#ifndef IS_SCENE_H_
#define IS_SCENE_H_

#include <vector>
#include <algorithm>

#include "node.hpp"
#include "nodes/text.hpp"

namespace is {

bool nodeCompareDepth( is::Node* a, is::Node* b );

class Scene {
    std::vector<is::Node*> m_nodes;
public:
    ~Scene();
    void                    tick( float dt );
    std::vector<is::Node*>  getSortedDrawables();
    std::vector<is::Node*>  getDrawables();
    void                    addNode( is::Node* );
    void                    draw();
    void                    remove( is::Node* );
};

};

extern is::Scene* world;
extern is::Scene* gui;

#endif // IS_SCENE_H_
