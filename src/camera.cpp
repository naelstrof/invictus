#include "camera.hpp"

is::Camera::Camera() {
    m_viewMatrix = glm::mat4(1);
    m_position = glm::vec3(0);
    // Face forward
    m_angle = glm::vec3( 0, 90 * ANG2RAD, 0 );
    m_changed = true;
    m_fov = 45;
    m_near = 0.1;
    m_far = 1000;
    m_ratio = 1;
    m_frustum = new Frustum( this );
}

glm::mat4 is::Camera::getViewMatrix() {
    update();
    return m_viewMatrix;
}

void is::Camera::update() {
    if ( !m_changed ) {
        return;
    }
    // Gotta convert our angle to a position... I hope glm::lookAt doesn't just convert it back :u
    glm::vec3 lookPos = glm::vec3( sin( m_angle.x ),
                                   cos( m_angle.x )*cos( m_angle.y ),
                                   cos( m_angle.x )*sin( m_angle.y ) );
    lookPos += m_position;
    m_viewMatrix = glm::lookAt( m_position, lookPos*10.f, glm::vec3( 0.f, 1.f, 0.f ) ); // +Y is always up.
    m_frustum->setCameraView( m_position, lookPos*10.f, glm::vec3( 0.f, 1.f, 0.f ) );
    m_changed = false;
}

glm::vec3 is::Camera::getPos() {
    return m_position;
}

void is::Camera::setPos( glm::vec3 pos ) {
    m_position = pos;
    m_changed = true;
}

void is::Camera::setPos( float x, float y, float z ) {
    m_position = glm::vec3( x, y, z );
    m_changed = true;
}

glm::vec3 is::Camera::getAng() {
    return m_angle;
}

void is::Camera::setAng( glm::vec3 ang ) {
    m_angle = ang;
    m_changed = true;
}

void is::Camera::setAng( float y, float p, float r ) {
    m_angle = glm::vec3( y, p, r );
    m_changed = true;
}

void is::Camera::setRatio( unsigned int w, unsigned int h ) {
    m_ratio = float( w ) / float( h );
}
