#include "text.hpp"

is::Text::Text( std::string text, std::string fontname ) {
    sf::Font* foo = font->get( fontname );
    m_text.setFont( *foo );
    m_text.setString( text );
    m_text.setCharacterSize( 30 );
    m_text.setColor( sf::Color::White );
    m_text.setPosition( 256, 256 );
}

is::Text::~Text() {

}

void is::Text::remove() {
    delete (is::Text*)this;
}

std::string is::Text::type() {
    return "text";
}

void is::Text::tick( float dt ) {
    // Do nothing! It's just text!
    //m_text.setPosition( m_text.getPosition() + 100*dt*( sf::Vector2f( (rand()%10), (rand()%10) ) - sf::Vector2f( (rand()%10), (rand()%10) ) ) );
}

void is::Text::draw( sf::RenderTarget* target ) {
    target->draw( m_text );
}
