#include "scene.hpp"

is::Scene* world = new is::Scene();
is::Scene* gui = new is::Scene();

bool is::nodeCompareDepth( is::Node* a, is::Node* b ) {
    return ( a->m_depth < b->m_depth );
}

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

std::vector<is::Node*> is::Scene::getSortedDrawables() {
    std::vector<is::Node*> drawables = getDrawables();
    std::sort( drawables.begin(), drawables.end(), is::nodeCompareDepth );
    return drawables;
}

std::vector<is::Node*> is::Scene::getDrawables() {
    std::vector<is::Node*> drawables;
    for ( unsigned int i=0;i<m_nodes.size();i++ ) {
        if ( m_nodes[i]->visible() ) {
            drawables.push_back( m_nodes[i] );
        }
    }
    return drawables;
}

void is::Scene::draw() {
    for ( unsigned int i=0;i<m_nodes.size();i++ ) {
        if ( m_nodes[i]->visible() ) {
            m_nodes[i]->draw();
        }
    }
}

void is::Scene::addNode( is::Node* node ) {
    m_nodes.push_back( node );
}
