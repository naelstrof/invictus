// texture.hpp: Loads textures into opengl

#ifndef IS_TEXTURE_H_
#define IS_TEXTURE_H_

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "textureloader.hpp"

#include "os.hpp"

namespace is {

class Animation {
public:
    Animation( std::string name );
    ~Animation();
    void bind( float time );
    sf::Texture* currentFrame( float time );
    void addFrame( std::string dir );
    std::string m_name;
    float m_fps;
    bool m_loop;
    bool m_rendered;
    void renderAll();
private:
    std::vector<sf::Texture*> m_frames;
};

class Texture {
public:
    Texture( std::string name, std::string dir );
    Texture( std::string name );
    ~Texture();
    void            tick( float dt );
    void            bind();
    void            unbind();
    void            addAnimation( is::Animation* animation );
    void            play( std::string name );
    unsigned int    getWidth();
    unsigned int    getHeight();
    std::string     m_name;
private:
    float           m_time;
    std::vector<is::Animation*> m_animations;
    unsigned int m_currentAnimation;
};

};

#endif // IS_TEXTURE_H_
