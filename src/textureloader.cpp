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

sf::Texture* is::TextureLoader::addRawTexture( std::string dir ) {
    is::File::Read file( dir );
    if ( !file.good() ) {
        os->printf( "ERR Couldn't find texture %!\n", dir );
        return NULL;
    }

    char* data = new char[ file.size() ];
    file.read( data, file.size() );

    sf::Texture* texture = new sf::Texture();
    texture->loadFromMemory( data, file.size() );

    delete[] data;

    m_rawTextures.push_back( texture );
    m_rawTextureDirs.push_back( dir );
    os->printf( "INF Loaded texture %.\n", dir );
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
