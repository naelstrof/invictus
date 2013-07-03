// shader.hpp: System that handles shader requests. It will dynamically search for and load shaders defined by lua.

#ifndef IS_SHADER_H_
#define IS_SHADER_H_

#include <vector>
#include <string>
#include <SFML/Graphics/Shader.hpp>

#include "lua.hpp"

namespace is {

class Shader {
private:
    std::vector<std::string> m_shadernames;
    std::vector<sf::Shader*> m_shaders;
public:
    Shader();
    ~Shader();
    int             init();
    sf::Shader*     get( std::string shadername );
    void            addShader( std::string name, std::string vertdir, std::string fragdir );
};

};

extern is::Shader* shader;

#endif // IS_SHADER_H_
