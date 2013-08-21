#include "texture.hpp"

is::Animation::Animation( std::string name )
    : m_name( name ), m_fps( 0 ), m_loop( true ), m_rendered( false ) {
}

is::Animation::~Animation() {
    // Textures will unallocate themselves on game shutdown.
}

sf::Texture* is::Animation::currentFrame( float time ) {
    renderAll();
    if ( m_loop ) {
        return m_frames[ fmod( time*m_fps, m_frames.size() ) ];
    }
    unsigned int location = abs( time*m_fps );
    if ( location < m_frames.size() ) {
        return m_frames[ abs( time*m_fps ) ];
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

void is::Animation::bind( float time ) {
    renderAll();
    if ( m_loop ) {
        sf::Texture::bind( m_frames[ fmod( time*m_fps, m_frames.size() ) ] );
        return;
    }
    unsigned int location = abs( time*m_fps );
    if ( location < m_frames.size() ) {
        sf::Texture::bind( m_frames[ abs( time*m_fps ) ] );
        return;
    }
    sf::Texture::bind( m_frames.back() );
}

void is::Animation::addFrame( std::string dir ) {
    m_frames.push_back( textures->getRaw( dir ) );
}

is::Texture::Texture( std::string name, std::string dir )
    : m_name( name ), m_currentAnimation( 0 ) {
    is::Animation* idle = new is::Animation( "idle" );
    idle->addFrame( dir );
    m_animations.push_back( idle );
}

is::Texture::Texture( std::string name )
    : m_name( name ), m_currentAnimation( 0 ) {
}

is::Texture::~Texture() {
    // Animations will unallocate themselves.
}

void is::Texture::tick( float dt ) {
    m_time += dt;
}

void is::Texture::bind() {
    m_animations[ m_currentAnimation ]->bind( m_time );
}

void is::Texture::addAnimation( is::Animation* animation ) {
    m_animations.push_back( animation );
}

void is::Texture::play( std::string name ) {
    for ( unsigned int i=0; i<m_animations.size(); i++ ) {
        if ( m_animations[i]->m_name == name ) {
            if ( i == m_currentAnimation ) {
                return;
            }
            m_currentAnimation = i;
            m_time = 0;
            return;
        }
    }
}

unsigned int is::Texture::getWidth() {
    return m_animations[ m_currentAnimation ]->currentFrame( m_time )->getSize().x;
}

unsigned int is::Texture::getHeight() {
    return m_animations[ m_currentAnimation ]->currentFrame( m_time )->getSize().y;
}
