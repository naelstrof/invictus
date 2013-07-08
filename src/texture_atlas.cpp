#include "texture_atlas.hpp"

is::TextureAtlas::TextureAtlas() {
    m_width = 0;
    m_height = 0;
}

is::TextureAtlas::TextureAtlas( unsigned int w, unsigned int h ) {
    m_width = w;
    m_height = h;

    m_node = Node( m_width, m_height );

    // Generate an empty texture to use as a texture atlas.
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &m_texture );
    unsigned char* data = new unsigned char[ m_width*m_height ];
    for ( unsigned int i=0; i<m_width*m_height; i++ ) {
        // Make sure the whole image is empty.
        data[i] = 0;
    }
    glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, m_width, m_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data );
    delete[] data;
}

is::TextureAtlas::~TextureAtlas() {
    glDeleteTextures( 1, &m_texture );
    // m_node should clean up itself automatically.
}

void is::TextureAtlas::bind() {
    glBindTexture( GL_TEXTURE_2D, m_texture );
}

is::TextureAtlas::Node* is::TextureAtlas::insert( unsigned int w, unsigned int h, unsigned char* imagedata ) {
    is::TextureAtlas::Node* node = m_node.insert( w, h );
    if ( !node ) {
        os->printf( "ERR Texture atlas has ran out of room! This needs to be fixed by a developer!\n" );
        return NULL;
    }

    glBindTexture( GL_TEXTURE_2D, m_texture );
    // Render the small texture to our atlas.
    glTexSubImage2D(GL_TEXTURE_2D, 0, node->m_rect.left, node->m_rect.top, w, h, GL_ALPHA, GL_UNSIGNED_BYTE, imagedata);
    return node;
}

is::TextureAtlas::Node::Node( unsigned int w, unsigned int h ) {
    m_children[0] = NULL;
    m_children[1] = NULL;
    m_img = 0;
    m_rect = sf::Rect<unsigned int>( 0, 0, w, h );
}

is::TextureAtlas::Node::Node() {
    m_children[0] = NULL;
    m_children[1] = NULL;
    m_img = 0;
    m_rect = sf::Rect<unsigned int>( 0, 0, 0, 0 );
}

is::TextureAtlas::Node::Node( unsigned int left, unsigned int top, unsigned int width, unsigned int height ) {
    m_children[0] = NULL;
    m_children[1] = NULL;
    m_img = 0;
    m_rect = sf::Rect<unsigned int>( left, top, width, height );
}

is::TextureAtlas::Node::~Node() {
    if ( m_children[0] != NULL ) {
        delete m_children[0];
        delete m_children[1];
    }
}

is::TextureAtlas::Node* is::TextureAtlas::Node::insert( unsigned int w, unsigned int h ) {
    // If we're not a leaf
    if ( m_children[0] != NULL ) {
        // Try inserting into first child
        is::TextureAtlas::Node* node = m_children[0]->insert( w, h );
        if ( node ) {
            return node;
        }

        // There was no room, attempt to insert into second.
        return m_children[1]->insert( w, h );
    }

    // If we already have an image stored here, return
    if ( m_img ) {
        return NULL;
    }
    // If we're too small, return
    if ( m_rect.width < w || m_rect.height < h ) {
        return NULL;
    }
    // We found a perfect fit!
    if ( m_rect.width == w || m_rect.height == h ) {
        m_img = true;
        return this;
    }

    // Ok, we have room, but we need to split to make it perfect.
    unsigned int dw = m_rect.width - w;
    unsigned int dh = m_rect.height - h;

    // Decide which way to split
    if ( dw > dh ) {
        // Vertical split, left node fits texture.
        m_children[0] = new is::TextureAtlas::Node( m_rect.left, m_rect.top, w, m_rect.height );
        m_children[1] = new is::TextureAtlas::Node( m_rect.left+w+1, m_rect.top, m_rect.width-w-1, m_rect.height);
    } else {
        // Horizontal split, top node fits texture.
        m_children[0] = new is::TextureAtlas::Node( m_rect.left, m_rect.top, m_rect.width, m_rect.height-h );
        m_children[1] = new is::TextureAtlas::Node( m_rect.left, m_rect.top-h-1, m_rect.width, m_rect.height-h-1);
    }

    // Now insert it into our top/left node
    return m_children[0]->insert( w, h );
}
