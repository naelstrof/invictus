#include "scene.hpp"

is::Scene* world = new is::Scene();
is::Scene* gui = new is::Scene();

is::Scene::~Scene() {
    for ( unsigned int i=0;i<m_nodes.size();i++ ) {
        m_nodes[i]->remove();
    }
}

void is::Scene::tick( float dt ) {
    for ( unsigned int i=0;i<m_nodes.size();i++ ) {
        m_nodes[i]->tick( dt );
    }
}

void is::Scene::draw() {
    for ( unsigned int i=0;i<m_nodes.size();i++ ) {
        m_nodes[i]->draw();
    }
}
