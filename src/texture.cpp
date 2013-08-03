#include "texture.hpp"

is::Animation::Animation( std::string name )
    : m_name( name ), m_fps( 0 ), m_ct( 0 ), m_loop( true ), m_rendered( false ) {
}

is::Animation::~Animation() {
    // Textures will unallocate themselves on game shutdown.
}

void is::Animation::tick( float dt ) {
    m_ct += dt;
}

sf::Texture* is::Animation::currentFrame() {
    renderAll();
    if ( m_loop ) {
        return m_frames[ fmod( m_ct*m_fps, m_frames.size() ) ];
    }
    unsigned int location = abs( m_ct*m_fps );
    if ( location < m_frames.size() ) {
        return m_frames[ abs( m_ct*m_fps ) ];
    }
    return m_frames.back();
}

void is::Animation::renderAll() {
    if ( m_rendered ) {
        return;
    }
    for ( unsigned int i=0; i<m_frames.size(); i++ ) {
        textures->render( m_frames.at( i ) );
    }
    m_rendered = true;
}

void is::Animation::bind() {
    renderAll();
    if ( m_loop ) {
        sf::Texture::bind( m_frames[ fmod( m_ct*m_fps, m_frames.size() ) ] );
        return;
    }
    unsigned int location = abs( m_ct*m_fps );
    if ( location < m_frames.size() ) {
        sf::Texture::bind( m_frames[ abs( m_ct*m_fps ) ] );
        return;
    }
    sf::Texture::bind( m_frames.back() );
}

void is::Animation::addFrame( std::string dir ) {
    m_frames.push_back( textures->getRaw( dir ) );
}

is::Texture::Texture( std::string name, std::string dir )
    : m_name( name ), m_currentAnimation( 0 ) {
    is::Animation idle( "idle" );
    idle.addFrame( dir );
    m_animations.push_back( idle );
}

is::Texture::Texture( std::string name )
    : m_name( name ), m_currentAnimation( 0 ) {
}

is::Texture::~Texture() {
    // Animations will unallocate themselves.
}

void is::Texture::tick( float dt ) {
    // Only tick the current animation.
    m_animations[ m_currentAnimation ].tick( dt );
}

void is::Texture::bind() {
    m_animations[ m_currentAnimation ].bind();
}

void is::Texture::addAnimation( is::Animation animation ) {
    m_animations.push_back( animation );
}

void is::Texture::play( std::string name ) {
    for ( unsigned int i=0; i<m_animations.size(); i++ ) {
        if ( m_animations[i].m_name == name ) {
            if ( i == m_currentAnimation ) {
                return;
            }
            m_currentAnimation = i;
            m_animations[ m_currentAnimation ].m_ct = 0;
            return;
        }
    }
}

unsigned int is::Texture::getWidth() {
    return m_animations[ m_currentAnimation ].currentFrame()->getSize().x;
}

unsigned int is::Texture::getHeight() {
    return m_animations[ m_currentAnimation ].currentFrame()->getSize().y;
}
