// texture_atlas.hpp: Object that packs and retrieves arbitrary textures into a single larger texture.

#ifndef IS_TEXTURE_ATLAS_H_
#define IS_TEXTURE_ATLAS_H_

namespace is {

class TextureAtlas {
public:
                                TextureAtlas();
                                ~TextureAtlas();
    class Node {
        public:
        Node();
        Node( unsigned int w, unsigned int h );
        Node( unsigned int top, unsigned int left, unsigned int right, unsigned int bottom );
        ~Node();
        Node*                   m_children[2];
        sf::Rect<unsigned int>  m_rect;
        bool                    m_img;
        int                     insert();
    };
    void                        bind();
private:
    unsigned int                m_texture;
    unsigned int                m_width;
    unsigned int                m_height;

    // Node is private since TextureAtlas should be the only one using it.
};

};

#endif // IS_TEXTURE_ATLAS_H_
