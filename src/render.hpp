// render.hpp: System that handles drawing both world and gui to screen.

#ifndef IS_RENDER_H_
#define IS_RENDER_H_

#include <SFML/Graphics/RenderTexture.hpp>

#include "window.hpp"
#include "scene.hpp"

namespace is {

class Render {
private:
    sf::RenderTexture m_world;
    sf::RenderTexture m_gui;
    bool m_sizeChanged;
public:
    Render();
    int             init();
    void            draw();
    void            updateSize();
};

};

extern is::Render* render;

#endif // IS_RENDER_H_
