// camera.hpp: Handles generating a View Matrix in the world.

#ifndef IS_CAMERA_H_
#define IS_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/euler_angles.hpp>

#define ANG2RAD 3.14159265358979323846/180.0

#include "frustum.hpp"

namespace is {

class Frustum;

class Camera {
public:
                Camera();
    glm::mat4   getViewMatrix();
    void        setPos( glm::vec3 pos );
    void        setAng( glm::vec3 ang );
    void        setPos( float x, float y, float z );
    void        setAng( float y, float p, float r );
    glm::vec3   getPos();
    glm::vec3   getAng();
    float       m_fov;
    float       m_near;
    float       m_far;
    float       m_ratio;
    void        setRatio( unsigned int w, unsigned int h );
    void        updateFrustum();
    // Unfortunately for the frustum to work effeciently, we need it to be inside the camera class. ( Due to transforming angles into a position in space ).
    is::Frustum* m_frustum;
private:
    bool        m_changed;
    void        update();
    glm::mat4   m_viewMatrix;
    glm::vec3   m_position;
    glm::vec3   m_angle;
};

};

#endif // IS_CAMERA_H_
