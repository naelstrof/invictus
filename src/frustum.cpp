#include "frustum.hpp"

is::Frustum::Frustum( is::Camera* cam ) {
    setCameraInternals( cam );
    m_camera = cam;
}

void is::Frustum::setCameraInternals( is::Camera* cam ) {
    m_ratio = cam->m_ratio;
    m_near = cam->m_near;
    m_far = cam->m_far;

    float angy = ANG2RAD * cam->m_fov * 0.5;

    m_tang = (float)tan( angy );
    m_sphereFactorY = 1.0/cos( angy );
    float angx = atan( m_tang*m_ratio );
    m_sphereFactorX = 1.0/cos( angx );

    m_height = m_near * m_tang;
    m_width = m_height * m_ratio;
    m_camera = cam;
}

void is::Frustum::setCameraView( glm::vec3 pos, glm::vec3 lookPos, glm::vec3 up ) {
    //glm::vec4 temp = view*glm::vec4( pos.x, pos.y, pos.z, 1 );
    //m_cameraPos = glm::vec3( -temp.x, temp.y, temp.z );
    m_cameraPos = pos;
    m_z = glm::normalize( lookPos - pos );
    m_x = glm::normalize( glm::cross( m_z, up ) );
    m_y = glm::cross( m_x, m_z );
}

int is::Frustum::pointInFrustum( glm::vec3 p, float* depth ) {
    float pcz, pcx, pcy, aux;

    // Compute vector from camera position to p
    glm::vec3 v = p - m_cameraPos;

    // Compute and test the Z coordinate
    pcz = glm::dot( v, m_z );
    if ( depth ) {
        *depth = pcz;
    }
    if ( pcz > m_far || pcz < m_near ) {
        return outside;
    }

    // Compute and test the Y coordinate
    pcy = glm::dot( v, m_y );
    aux = pcz * m_tang;
    if ( pcy > aux || pcy < -aux ) {
        return outside;
    }

    // Compute and test the X coordinate
    pcx = glm::dot( v, m_x );
    aux = aux * m_ratio;
    if ( pcx > aux || pcx < -aux ) {
        return outside;
    }

    //os->printf( "SUCCESS %, %, %. ", pcx, pcy, pcz );
    return inside;
}

int is::Frustum::sphereInFrustum( glm::vec3 p, float r, float* depth ) {
    float d;
    float az, ax, ay;
    int result = inside;
    glm::vec3 v = p - m_cameraPos;

    az = glm::dot( v, m_z );
    if ( depth ) {
        *depth = az;
    }
    if ( az > m_far + r || az < m_near-r ) {
        return outside;
    }
    if ( az > m_far - r || az < m_near + r ) {
        result = intersect;
    }

    ay = glm::dot( v, m_y );
    d = m_sphereFactorY * r;
    az *= m_tang;
    if ( ay > az + d || ay < -az - d ) {
        return outside;
    }
    if ( ay > az - d || ay < -az + d ) {
        result = intersect;
    }

    ax = glm::dot( v, m_x );
    d = m_sphereFactorX * r;
    az *= m_ratio;
    if ( ax > az + d || ax < -az - d ) {
        return outside;
    }
    if ( ax > az - d || ax < -az + d ) {
        result = intersect;
    }
    return result;
}
