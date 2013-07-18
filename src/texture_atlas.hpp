// texture_atlas.hpp: Object that packs and retrieves arbitrary textures into a single larger texture.

#ifndef IS_TEXTURE_ATLAS_H_
#define IS_TEXTURE_ATLAS_H_

#include <SFML/Graphics/Rect.hpp>
#include <GL/glew.h>

#include "os.hpp"

namespace is {

class TextureAtlas {
public:
    class Node {
        public:
        Node();
        Node( Node* smallernode,  unsigned int w, unsigned int h );
        Node( unsigned int w, unsigned int h );
        Node( unsigned int top, unsigned int left, unsigned int width, unsigned int height );
        ~Node();
        Node*                   m_children[2];
        sf::Rect<unsigned int>  m_rect;
        bool                    m_img;
        is::TextureAtlas::Node* insert( unsigned int w, unsigned int h );
        is::TextureAtlas::Node* insert( Node* node );
        void                    localize( Node* node );
    };

    TextureAtlas();
    TextureAtlas( unsigned int w, unsigned int h );
    ~TextureAtlas();
    void                        bind();
    is::TextureAtlas::Node*     insert( unsigned int w, unsigned int h, unsigned char* imagedata, int padding = 0 );
    unsigned int                m_width;
    unsigned int                m_height;
    bool                        changed();
private:
    bool                        m_changed;
    Node*                       m_node;
    unsigned int                m_texture;

    // Node is private since TextureAtlas should be the only one using it.
};

};

#endif // IS_TEXTURE_ATLAS_H_
