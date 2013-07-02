// shader.hpp: System that handles shader requests. It will dynamically search for and load shaders defined by lua.

#ifndef IS_SHADER_H_
#define IS_SHADER_H_

namespace is {

class Shader {
public:
    Shader();
    ~Shader();
    int             init();
    void            bind( std::string shader );
};

};

extern is::Shader* render;

#endif // IS_SHADER_H_
