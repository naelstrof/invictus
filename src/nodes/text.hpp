// text.hpp: Text renderable

#ifndef IS_TEXT_H_
#define IS_TEXT_H_

#include <GL/glew.h>
#include <SFML/System/Utf.hpp>

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
    void            draw();
    sf::String      m_text;
    std::string     m_font;
    unsigned int    m_size;
    bool            m_changed;
    void            setSize( unsigned int size );
private:
    is::TextureAtlas*       m_texture;
    unsigned int            m_vertcount;
    float                   m_totaltime;
    std::vector<glm::vec2>  m_uvs;
    std::vector<glm::vec2>  m_verts;
    unsigned int            m_buffers[2];
    void                    generateBuffers();
    unsigned int            m_textureSize;
};

};

#endif
