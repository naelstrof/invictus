// text.hpp: Text renderable

#ifndef IS_TEXT_H_
#define IS_TEXT_H_

#include <GL/glew.h>
#include <glm/gtx/projection.hpp>

#include "../glyph.hpp"
#include "../window.hpp"
#include "../node.hpp"

namespace is {

class Text : public is::Node {
public:
                    Text( sf::String text="", std::string fontname="gui", int size=32 );
                    ~Text();
    void            remove();
    std::string     type();
    void            tick( float dt );
    void            draw( sf::RenderTarget* target );
    std::string     m_font;
    sf::String      m_text;
    bool            m_changed;
    unsigned int    m_size;
    void            setSize( unsigned int size );
private:
    std::vector<glm::vec2>  m_uvs;
    std::vector<glm::vec2>  m_verts;
    unsigned int            m_vertcount;
    unsigned int            m_buffers[2];
    is::TextureAtlas*       m_texture;
};

};

#endif
