#include "node.hpp"

is::Node::Node() {
    m_color = glm::vec4( 1 );
    m_position = glm::vec3( 0 );
    m_angle = glm::vec3( 0 );
    m_scale = glm::vec3( 1 );
    m_matrix = glm::mat4( 1 );
    m_parent = NULL;
    m_matrixChanged = false;
    m_depth = 0;
    m_hullsize = 0;
    m_luaReference = LUA_NOREF;
}

is::Node::~Node() {
    for( unsigned int i=0;i<m_children.size();i++ ) {
        m_children[i]->setParent( NULL );
    }
    if ( m_parent != NULL ) {
        m_parent->removeChild( this );
    }
}

is::Node* is::Node::getParent() {
    return m_parent;
}

void is::Node::setParent( is::Node* node ) {
    if ( m_parent != NULL && node != NULL ) {
        m_parent->removeChild( this );
    }
    m_parent = node;
    if ( node == NULL ) {
        return;
    }
    m_parent->addChild( this );
    //Make sure to update the matrix on the next update.
    m_matrixChanged = true;
}

std::string is::Node::type() {
    return "None";
}

void is::Node::tick( float dt ) {
}

void is::Node::draw() {
}

bool is::Node::visible() {
    return false;
}

void is::Node::removeChild( is::Node* node ) {
    for( unsigned int i=0; i<m_children.size(); i++ ) {
        if ( m_children[i] == node ) {
            m_children.erase( m_children.begin() + i );
            return;
        }
    }
    os->printf( "WRN is::Node::removeChild( is::Node* ) failed! Specified child doesn't exist!\n" );
}

void is::Node::addChild( is::Node* node ) {
    //Make sure child's parent is this node.
    if ( node->getParent() == NULL ) {
        node->setParent( this );
    }
    //Make sure the child doesn't already exist.
    for( unsigned int i=0;i<m_children.size();i++ ) {
        if ( m_children[i] == node ) {
            os->printf( "is::Node::addChild( is::Node* ) failed! Specified child already exists!\n" );
            return;
        }
    }
    m_children.push_back( node );
}

is::Node* is::Node::getRoot() {
    if ( m_parent == NULL ) {
        return this;
    }
    return m_parent->getRoot();
}

glm::vec3 is::Node::getPos() {
    if ( m_parent ) {
        return m_parent->getPos()+m_position;
    }
    return m_position;
}

void is::Node::setPos( glm::vec3 pos ) {
    if ( m_position == pos ) {
        return;
    }
    m_position = pos;
    setMatrixChanged( true );
}

void is::Node::setPos( float x, float y, float z ) {
    if ( m_position == glm::vec3( x, y, z ) ) {
        return;
    }
    m_position = glm::vec3( x, y, z );
    setMatrixChanged( true );
}

glm::vec3 is::Node::getAng() {
    if ( m_parent ) {
        return m_parent->getAng()+m_angle;
    }
    return m_angle;
}

void is::Node::setAng( glm::vec3 ang ) {
    m_angle = ang;
    setMatrixChanged( true );
}

void is::Node::setAng( float y, float p, float r ) {
    m_angle = glm::vec3( y, p, r );
    setMatrixChanged( true );
}

glm::vec3 is::Node::getScale() {
    return m_scale;
}

void is::Node::setScale(glm::vec3 scale) {
    if ( m_scale == scale ) {
        return;
    }
    m_scale = scale;
    setMatrixChanged( true );
}

void is::Node::setScale( float w, float h, float d ) {
    if ( m_scale == glm::vec3( w, h ,d ) ) {
        return;
    }
    m_scale = glm::vec3( w, h, d );
    setMatrixChanged( true );
}

void is::Node::setMatrixChanged( bool changed ) {
    for( unsigned int i=0;i<m_children.size();i++ ) {
        m_children[i]->setMatrixChanged( changed );
    }
    m_matrixChanged = changed;
}

glm::vec4 is::Node::getColor() {
    return m_color;
}

void is::Node::setColor( glm::vec4 color ) {
    m_color = color;
}

void is::Node::setColor( float r, float g, float b, float a ) {
    m_color = glm::vec4( r, g, b, a );
}

glm::mat4 is::Node::getScalelessMatrix() {
    // Make sure we have the matrix generated.
    getModelMatrix();
    return m_noscaleMatrix;
}

glm::mat4 is::Node::getModelMatrix() {
    //If we have a cached matrix, then just return that.
    if ( !m_matrixChanged ) {
        return m_matrix;
    }
    m_matrixChanged = false;
    //If we have a parent, use their matrix offset by our local values.
    if ( m_parent ) {
        m_matrix = m_parent->getScalelessMatrix();
        m_matrix = glm::translate( m_matrix, m_position );
        m_matrix = m_matrix * glm::eulerAngleYXZ( m_angle.x, m_angle.y, m_angle.z );
        m_noscaleMatrix = m_matrix;
        m_matrix = glm::scale( m_matrix, m_scale );
        return m_matrix;
    }
    //Otherwise just generate a new matrix.
    m_matrix = glm::translate( glm::mat4(1), m_position );
    m_matrix = m_matrix * glm::eulerAngleYXZ( m_angle.x, m_angle.y, m_angle.z );
    m_noscaleMatrix = m_matrix;
    m_matrix = glm::scale( m_matrix, m_scale );
    return m_matrix;
}

void is::Node::remove() {
    delete (is::Node*)this;
}
