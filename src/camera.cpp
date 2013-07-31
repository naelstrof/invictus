#include "camera.hpp"

is::Camera::Camera() {
    m_viewMatrix = glm::mat4(1);
}

glm::mat4 is::Camera::getViewMatrix() {
    updateViewMatrix();
    return m_viewMatrix;
}

void is::Camera::updateViewMatrix() {
    // Create a backwards view matrix based on this node's model matrix, and use it as the view.

    //m_viewMatrix = glm::translate( glm::mat4(1), -getPos() );
    //m_viewMatrix = glm::scale( m_viewMatrix, getScale() ); // Not sure if we should scale the view like this.
    //glm::vec3 angle = getAng();
    //m_viewMatrix = glm::eulerAngleYXZ( -angle.x, -angle.y, -angle.z ) * m_viewMatrix;

    m_viewMatrix = glm::inverse( getModelMatrix() );
}
