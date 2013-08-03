#include "textureloader.hpp"

is::TextureLoader* textures = new is::TextureLoader();

is::TextureLoader::TextureLoader() {

}

is::TextureLoader::~TextureLoader() {
    for ( unsigned int i=0; i<m_textures.size(); i++ ) {
        delete m_rawTextures[i];
        delete m_textures[i];
    }
}

int is::TextureLoader::init() {
    // Not actually used to load actual pixel data, but actual textures with animations.
    lua->doFolder( "data/textures" );
    return 0;
}

sf::Texture* is::TextureLoader::getRaw( std::string dir ) {
    for ( unsigned int i=0; i<m_rawTextures.size(); i++ ) {
        if ( dir == m_rawTextureDirs[i] ) {
            return m_rawTextures[i];
        }
    }
    return addRawTexture( dir );
}

void is::TextureLoader::render( sf::Texture* texture ) {
    for ( unsigned int i=0; i<m_rawTextures.size(); i++ ) {
        if ( texture == m_rawTextures[i] && m_rawTextureDirs[i] != "" ) {

            is::File::Read file( m_rawTextureDirs[i] );
            if ( !file.good() ) {
                os->printf( "ERR Couldn't find texture %!\n", m_rawTextureDirs[i] );
                m_rawTextureDirs[i] = "";
                return;
            }
            char* data = new char[ file.size() ];
            file.read( data, file.size() );

            bool success = texture->loadFromMemory( data, file.size() );
            if ( !success ) {
                os->printf( "ERR SFML failed to load texture %! It will appear black.\n", m_rawTextureDirs[i]);
            }

            delete[] data;
            os->printf( "INF Loaded texture %.\n", m_rawTextureDirs[i] );
            m_rawTextureDirs[i] = "";
            return;

        }
    }
}

sf::Texture* is::TextureLoader::addRawTexture( std::string dir ) {

    sf::Texture* texture = new sf::Texture();
    m_rawTextures.push_back( texture );
    m_rawTextureDirs.push_back( dir );
    return texture;
}

is::Texture* is::TextureLoader::get( std::string name ) {
    for ( unsigned int i=0; i<m_textures.size(); i++ ) {
        if ( name == m_textures[i]->m_name ) {
            return m_textures[i];
        }
    }
    os->printf( "ERR Couldn't find texture %!\n", name );
    return NULL;
}

void is::TextureLoader::addTexture( is::Texture* texture ) {
    m_textures.push_back( texture );
}
