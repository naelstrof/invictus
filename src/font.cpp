#include "font.hpp"

is::Font* fonts = new is::Font();
FT_Library ftlib = NULL;

is::Font::Font() {
}

is::Font::~Font() {
    FT_Done_FreeType( ftlib );
    for ( unsigned int i=0; i<m_fonts.size(); i++ ) {
        if ( m_fonts[i].m_data != NULL ) {
            delete[] m_fonts[i].m_data;
        }
    }
}

int is::Font::init() {
    int err = FT_Init_FreeType( &ftlib );
    if ( err ) {
        os->printf( "ERR Freetype failed to initialize!\n" );
        return 1;
    }
    lua->doFolder( "data/fonts" );
    return 0;
}

is::FontStore* is::Font::get( std::string fontname ) {
    for ( unsigned int i=0; i<m_fonts.size(); i++ ) {
        if ( fontname == m_fonts[i].m_name ) {
            loadFont( i );
            return &( m_fonts.at( i ) );
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
    if ( font.m_face != NULL ) {
        return;
    }

    is::File::Read file( font.m_dir );

    char* data = new char[ file.size() ];
    file.read( data, file.size() );

    //m_fonts.at( id ).m_font = new sf::Font();
    //m_fonts.at( id ).m_font->loadFromMemory( data, file.size() );
    int err = FT_New_Memory_Face( ftlib, (const unsigned char*)data, file.size(), 0, & ( m_fonts.at( id ).m_face ) );
    if ( err ) {
        os->printf( "ERR Error loading font: %. It's either corrupt, not a font, or otherwise unreadable!\n", font.m_dir );
        return;
    }

    // Can't delete the data until it's not being used anymore :/
    m_fonts.at( id ).m_data = data;
    //delete[] data;


    os->printf( "INF Loaded font % as %.\n", font.m_name, font.m_dir );
}

is::FontStore::FontStore( std::string name, std::string dir, FT_Face font ) {
    m_name = name;
    m_dir = dir;
    m_face = font;
    m_data = NULL;
}
