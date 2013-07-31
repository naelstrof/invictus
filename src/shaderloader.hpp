// shaderloader.hpp: System that handles shader requests. It will dynamically search for and load shaders defined by lua.

#ifndef IS_SHADERLOADER_H_
#define IS_SHADERLOADER_H_

#include <vector>
#include <string>

#include "shader.hpp"
#include "lua.hpp"

namespace is {

class ShaderLoader {
private:
    std::vector<is::Shader*> m_shaders;
public:
    ShaderLoader();
    ~ShaderLoader();
    int                         init();
    is::Shader*                 get( std::string shadername );
    void                        addShader( std::string name, std::string vertdir, std::string fragdir, std::string type );
    std::vector<is::Shader*>    getAll();
};

};

extern is::ShaderLoader* shaders;

#endif // IS_SHADERLOADER_H_
