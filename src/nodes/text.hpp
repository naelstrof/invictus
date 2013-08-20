// text.hpp: Text renderable

#ifndef IS_TEXT_H_
#define IS_TEXT_H_

#include <GL/glew.h>
#include <algorithm>

#include "../render.hpp"
#include "../camera.hpp"
#include "../glyph.hpp"
#include "../node.hpp"

namespace is {

class Text : public is::Node {
public:
    enum RenderMode {
        Left,
        Middle,
        Right
    };
                    Text( sf::String text="", std::string fontname="gui", int size=40 );
                    ~Text();
    void            remove();
    std::string     type();
    void            tick( float dt );
    void            draw();
    bool            visible();
    sf::String      m_text;
    std::string     m_font;
    unsigned int    m_size;
    bool            m_changed;
    float           m_width;
    float           m_height;
    is::Shader*     m_shader;
    void            setSize( unsigned int size );
    void            setText( sf::String text );
    std::string     getText();
    RenderMode      m_renderMode;
private:
    is::TextureAtlas*       m_texture;
    unsigned int            m_vertcount;
    unsigned int            m_buffers[2];
    void                    generateBuffers();
    void                    getDimensions();
    unsigned int            m_textureSize;
};

};

#endif
