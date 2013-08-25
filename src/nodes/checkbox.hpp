// checkbox.hpp: Checkbox renderable

#ifndef IS_CHECKBOX_H_
#define IS_CHECKBOX_H_

#include <GL/glew.h>
#include <SFML/System/Utf.hpp>

#include "../render.hpp"
#include "../camera.hpp"
#include "../texture.hpp"
#include "../node.hpp"
#include "../mouse.hpp"

namespace is {

class Checkbox : public is::Node {
public:
                    Checkbox( std::string texturename, float borderSize = 0, bool checked = false );
                    ~Checkbox();
    void            remove();
    std::string     type();
    void            tick( float dt );
    void            draw();
    bool            visible();
    void            play( std::string );
    is::Texture*    m_texture;
    is::Shader*     m_shader;
    bool            m_changed;
    void            setScale( glm::vec3 scale );
    void            setScale( float w, float h, float d );
    int             m_luaCheckFunction;
    int             m_luaUncheckFunction;
    bool            intersects( glm::vec3 pos );
    bool            m_checked;
private:
    bool            m_pressed;
    bool            m_released;
    float           m_border;
    unsigned int    m_vertcount;
    unsigned int    m_buffers[2];
    void            generateBuffers();
};

};

#endif
