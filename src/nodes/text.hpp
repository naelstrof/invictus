// text.hpp: Text renderable

#ifndef IS_TEXT_H_
#define IS_TEXT_H_

#include <SFML/Graphics/Text.hpp>
#include "../font.hpp"
#include "../window.hpp"
#include "../node.hpp"

namespace is {

class Text : public is::Node {
public:
                    Text( std::string text, std::string fontname );
                    ~Text();
    void            remove();
    std::string     type();
    void            tick( float dt );
    void            draw( sf::RenderTarget* target );
    sf::Text        m_text;
};

};

#endif
