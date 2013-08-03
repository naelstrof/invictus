// render.hpp: System that handles drawing both world and gui to screen.

#ifndef IS_RENDER_H_
#define IS_RENDER_H_

#include <SFML/Graphics/RenderTexture.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "window.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "framebuffer.hpp"

namespace is {

class Camera;

class Render {
private:
    void                    updateCamera( float distance );
    is::Framebuffer         m_buffer;
    glm::mat4               m_perspMatrix;
    glm::mat4               m_orthoMatrix;
public:
    Render();
    ~Render();
    int                     init();
    void                    draw();
    void                    tick();
    is::Camera*             m_camera;
    std::vector<is::Node*>  m_drawables;
};

};

extern is::Render* render;

#endif // IS_RENDER_H_
