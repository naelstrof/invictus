// button.hpp: Button renderable

#ifndef IS_BUTTON_H_
#define IS_BUTTON_H_

#include <GL/glew.h>
#include <SFML/System/Utf.hpp>

#include "../render.hpp"
#include "../camera.hpp"
#include "../texture.hpp"
#include "../node.hpp"
#include "../mouse.hpp"

namespace is {

class Button : public is::Node {
public:
                    Button( std::string texturename, float borderSize = 0 );
                    ~Button();
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
    int             m_luaPressFunction;
    int             m_luaReleaseFunction;
    bool            intersects( glm::vec3 pos );
private:
    float           m_border;
    unsigned int    m_vertcount;
    unsigned int    m_buffers[2];
    void            generateBuffers();
};

};

#endif
