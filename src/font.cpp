#include "font.hpp"

is::Font* font = new is::Font();

is::Font::Font() {

}

is::Font::~Font() {
    for ( unsigned int i=0; i<m_fonts.size(); i++ ) {
        delete m_fonts[i].m_font;
        delete[] m_fonts[i].m_data;
    }
}

int is::Font::init() {
    lua->doFolder( "data/fonts" );
    return 0;
}

sf::Font* is::Font::get( std::string fontname ) {
    for ( unsigned int i=0; i<m_fonts.size(); i++ ) {
        if ( fontname == m_fonts[i].m_name ) {
            loadFont( i );
            return m_fonts.at( i ).m_font;
        }
    }
    os->printf( "ERR Couldn't find font %!\n", fontname );
    // TODO: Return a default font here so we don't crash.
    return NULL;
}

// Adding a font just adds its information. Once it's requested it will be loaded.
void is::Font::addFont( std::string name, std::string dir ) {
    m_fonts.push_back( is::FontStore( name, dir, NULL ) );
}

void is::Font::loadFont( int id ) {
    is::FontStore font = m_fonts.at( id );
    if ( font.m_font != NULL ) {
        return;
    }

    is::File::Read file( font.m_dir );

    char* data = new char[ file.size() ];
    file.read( data, file.size() );

    m_fonts.at( id ).m_font = new sf::Font();
    m_fonts.at( id ).m_font->loadFromMemory( data, file.size() );

    // Can't delete the data until it's not being used anymore :/
    m_fonts.at( id ).m_data = data;
    //delete[] data;


    os->printf( "INF Loaded font % as %.\n", font.m_name, font.m_dir );
}

is::FontStore::FontStore( std::string name, std::string dir, sf::Font* font ) {
    m_name = name;
    m_dir = dir;
    m_font = font;
}
