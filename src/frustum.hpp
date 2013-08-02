// frustum.hpp: Handles testing if objects are visible within a view frustum.

#ifndef IS_FRUSTUM_H_
#define IS_FRUSTUM_H_

#define ANG2RAD 3.14159265358979323846/180.0

#include <glm/glm.hpp>

#include "os.hpp"
#include "render.hpp"
#include "camera.hpp"

namespace is {

class Camera;

class Frustum {
public:
    enum { outside, intersect, inside };
    Frustum( is::Camera* cam );
    void setCameraInternals( is::Camera* cam );
    void setCameraView( glm::vec3 pos, glm::vec3 lookPos, glm::vec3 up );
    int  pointInFrustum( glm::vec3 p, float* depth = NULL );
    int  sphereInFrustum( glm::vec3 p, float r, float* depth = NULL );
private:
    glm::vec3   m_cameraPos;
    glm::vec3   m_x, m_y, m_z;
    float       m_near, m_far;
    float       m_width, m_height;
    float       m_ratio;
    float       m_tang;
    float       m_sphereFactorY;
    float       m_sphereFactorX;
    is::Camera* m_camera;
};

};

#endif // IS_FRUSTUM_H_
