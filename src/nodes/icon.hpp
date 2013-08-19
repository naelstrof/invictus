// icon.hpp: Icon renderable

#ifndef IS_ICON_H_
#define IS_ICON_H_

#include <GL/glew.h>
#include <SFML/System/Utf.hpp>

#include "../render.hpp"
#include "../camera.hpp"
#include "../texture.hpp"
#include "../node.hpp"

namespace is {

class Icon : public is::Node {
public:
                    Icon( std::string texturename );
                    ~Icon();
    void            remove();
    std::string     type();
    void            tick( float dt );
    void            draw();
    bool            visible();
    void            play( std::string );
    is::Texture*    m_texture;
    is::Shader*     m_shader;
    bool            m_changed;
private:
    unsigned int    m_vertcount;
    unsigned int    m_buffers[2];
    void            generateBuffers();
};

};

#endif
