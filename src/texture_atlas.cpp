#include "texture_atlas.hpp"

is::TextureAtlas::TextureAtlas() {
    m_width = 0;
    m_height = 0;
    m_texture = 0;
    m_node = NULL;
}

is::TextureAtlas::TextureAtlas( unsigned int w, unsigned int h ) {
    m_width = w;
    m_height = h;

    m_node = new Node( m_width, m_height );

    // Generate an empty texture to use as a texture atlas.
    glGenTextures( 1, &m_texture );
    glBindTexture( GL_TEXTURE_2D, m_texture );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexStorage2D( GL_TEXTURE_2D, 1, GL_R8, m_width, m_height );
}

is::TextureAtlas::~TextureAtlas() {
    glDeleteTextures( 1, &m_texture );
    delete m_node;
}

void is::TextureAtlas::bind() {
    glBindTexture( GL_TEXTURE_2D, m_texture );
}

is::TextureAtlas::Node* is::TextureAtlas::insert( unsigned int w, unsigned int h, unsigned char* imagedata ) {
    // We don't want to explode the video memory by accident.
    if ( w > 4028 || h > 4028 ) {
        os->printf( "ERR Texture atlas overflow, please don't insert huge images into it!\n" );
    }

    is::TextureAtlas::Node* node = m_node->insert( w, h );
    // Continually resize the texture until we can fit the requested texture.
    unsigned int oldw = m_width;
    unsigned int oldh = m_height;
    while ( !node ) {
        // We must have ran out of room in the texture, automatically double in size.
        m_width *= 2;
        m_height *= 2;
        is::TextureAtlas::Node* newnode = new Node( m_width, m_height );
        is::TextureAtlas::Node* oldnode = newnode->insert( m_node );
        m_node = newnode;

        node = m_node->insert( w, h );
        if ( !node ) {
            continue;
        }

        // Oh yeah and recreate the texture.
        // First start by copying the original texture to memory.
        unsigned char* oldtexture = new unsigned char[oldw*oldh];
        glBindTexture( GL_TEXTURE_2D, m_texture );
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGetTexImage( GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, oldtexture );
        glDeleteTextures( 1, &m_texture );

        // Then recreate the texture and write the original texture back onto the newly sized texture.
        glGenTextures( 1, &m_texture );
        glBindTexture( GL_TEXTURE_2D, m_texture );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexStorage2D( GL_TEXTURE_2D, 1, GL_R8, m_width, m_height );
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D( GL_TEXTURE_2D, 0, oldnode->m_rect.left, oldnode->m_rect.top-oldnode->m_rect.height, oldw, oldh, GL_RED, GL_UNSIGNED_BYTE, oldtexture );
        delete[] oldtexture;
    }


    glBindTexture( GL_TEXTURE_2D, m_texture );
    // Render the small texture to our atlas.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D( GL_TEXTURE_2D, 0, node->m_rect.left, node->m_rect.top-h, w, h, GL_RED, GL_UNSIGNED_BYTE, imagedata );
    return node;
}

is::TextureAtlas::Node::Node( unsigned int w, unsigned int h ) {
    m_children[0] = NULL;
    m_children[1] = NULL;
    m_img = 0;
    m_rect = sf::Rect<unsigned int>( 0, h, w, h );
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
    if ( m_rect.width == w && m_rect.height == h ) {
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
        m_children[0] = new is::TextureAtlas::Node( m_rect.left, m_rect.top, m_rect.width, h );
        m_children[1] = new is::TextureAtlas::Node( m_rect.left, m_rect.top-h-1, m_rect.width, m_rect.height-h-1);
    }

    // Now insert it into our top/left node
    return m_children[0]->insert( w, h );
}

// Same as inserting a rectangle but instead we insert a whole node tree, this is used so we can resize the texture atlas.
is::TextureAtlas::Node* is::TextureAtlas::Node::insert( is::TextureAtlas::Node* newnode ) {
    if ( m_children[0] != NULL ) {
        is::TextureAtlas::Node* node = m_children[0]->insert( newnode );
        if ( node ) {
            return node;
        }
        return m_children[1]->insert( newnode );
    }
    if ( m_img ) {
        return NULL;
    }
    unsigned int w = newnode->m_rect.width;
    unsigned int h = newnode->m_rect.height;
    if ( m_rect.width < w || m_rect.height < h ) {
        return NULL;
    }
    if ( m_rect.width == w && m_rect.height == h ) {
        // FIXME: Instead of removing the original node we can actually stick it inside the tree without messing with children and deleting parents, but this is so much easier :u.
        // Since we're inserting a node, we first recursively localize the values.
        localize( newnode );
        // Then we take its children ( lol )
        m_children[0] = newnode->m_children[0];
        m_children[1] = newnode->m_children[1];
        newnode->m_children[0] = NULL;
        newnode->m_children[1] = NULL;
        // Then we murder the parent.
        delete newnode;

        m_img = true;
        return this;
    }
    unsigned int dw = m_rect.width - w;
    unsigned int dh = m_rect.height - h;
    if ( dw > dh ) {
        m_children[0] = new is::TextureAtlas::Node( m_rect.left, m_rect.top, w, m_rect.height );
        m_children[1] = new is::TextureAtlas::Node( m_rect.left+w+1, m_rect.top, m_rect.width-w-1, m_rect.height);
    } else {
        m_children[0] = new is::TextureAtlas::Node( m_rect.left, m_rect.top, m_rect.width, h );
        m_children[1] = new is::TextureAtlas::Node( m_rect.left, m_rect.top-h-1, m_rect.width, m_rect.height-h-1);
    }
    return m_children[0]->insert( newnode );
}

// This is used to put a node inside of another, it simply recursively offsets a node tree with another node's settings.
void is::TextureAtlas::Node::localize( is::TextureAtlas::Node* node ) {
    node->m_rect.left += m_rect.left;
    node->m_rect.top += m_rect.top-m_rect.height;
    if ( node->m_children[0] ) {
        localize( node->m_children[0] );
        localize( node->m_children[1] );
    }
}
