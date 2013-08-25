// button.hpp: Dropdown renderable

#ifndef IS_DROPDOWN_H_
#define IS_DROPDOWN_H_

#include <GL/glew.h>
#include <SFML/System/Utf.hpp>
#include <vector>
#include <string>

#include "../render.hpp"
#include "../camera.hpp"
#include "../texture.hpp"
#include "../node.hpp"
#include "../mouse.hpp"

namespace is {

class Dropdown : public is::Node {
public:
                    Dropdown( std::string texturename, float borderSize = 0 );
                    ~Dropdown();
    void            remove();
    std::string     type();
    void            tick( float dt );
    void            draw();
    bool            visible();
    void            play( std::string );
    is::Texture*    m_texture;
    is::Shader*     m_shader;
    is::Text*       m_text;
    bool            m_changed;
    void            setScale( glm::vec3 scale );
    void            setScale( float w, float h, float d );
    int             m_luaChangeFunction;
    bool            intersects( glm::vec3 pos );
    std::vector<std::string> m_elements;
    unsigned int    addRow( std::string row );
    void            selectElement( unsigned int element );
    unsigned int    m_selectedElement;
private:
    float           m_stretched;
    bool            m_active;
    bool            m_pressed;
    bool            m_released;
    float           m_border;
    unsigned int    m_vertcount;
    unsigned int    m_buffers[2];
    void            generateBuffers();
};

};

#endif
