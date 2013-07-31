// camera.hpp: Handles generating a View Matrix in the world.

#ifndef IS_CAMERA_H_
#define IS_CAMERA_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "os.hpp"
#include "node.hpp"

namespace is {

class Camera : public is::Node {
public:
                Camera();
    glm::mat4   getViewMatrix();
private:
    void        updateViewMatrix();
    glm::mat4   m_viewMatrix;
};

};

#endif // IS_CAMERA_H_
