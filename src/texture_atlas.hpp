// texture_atlas.hpp: Object that packs and retrieves arbitrary textures into a single larger texture.

#ifndef IS_TEXTURE_ATLAS_H_
#define IS_TEXTURE_ATLAS_H_

#include <SFML/Graphics/Rect.hpp>
#include <GL/gl.h>

#include "os.hpp"

namespace is {

class TextureAtlas {
public:
    class Node {
        public:
        Node();
        Node( unsigned int w, unsigned int h );
        Node( unsigned int top, unsigned int left, unsigned int width, unsigned int height );
        ~Node();
        Node*                   m_children[2];
        sf::Rect<unsigned int>  m_rect;
        bool                    m_img;
        is::TextureAtlas::Node* insert( unsigned int w, unsigned int h );
    };

    TextureAtlas();
    TextureAtlas( unsigned int w, unsigned int h );
    ~TextureAtlas();
    void                        bind();
    is::TextureAtlas::Node*     insert( unsigned int w, unsigned int h, unsigned char* imagedata );
    unsigned int                m_width;
    unsigned int                m_height;
private:
    Node                        m_node;
    unsigned int                m_texture;

    // Node is private since TextureAtlas should be the only one using it.
};

};

#endif // IS_TEXTURE_ATLAS_H_
