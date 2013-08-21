// textureloader.hpp: System that handles texture requests. It will dynamically search for and load textures defined by lua.

#ifndef IS_TEXTURELOADER_H_
#define IS_TEXTURELOADER_H_

#include <vector>
#include <string>
#include <SFML/Graphics/Texture.hpp>

#include "texture.hpp"
#include "lua.hpp"

namespace is {

class Texture;
class Animation;

class TextureLoader {
private:
    std::vector<is::Texture*>     m_textures;
    std::vector<std::string>      m_rawTextureDirs;
    std::vector<sf::Texture*>     m_rawTextures;
    std::vector<is::Animation*>   m_animations;
public:
    TextureLoader();
    ~TextureLoader();
    int                         init();
    is::Texture*                get( std::string dir );
    void                        addTexture( is::Texture* texture );
    sf::Texture*                getRaw( std::string dir );
    sf::Texture*                addRawTexture( std::string dir );
    void                        addAnimation( is::Animation* animation );
    void                        render( sf::Texture* );
};

};

extern is::TextureLoader* textures;

#endif // IS_TEXTURELOADER_H_
