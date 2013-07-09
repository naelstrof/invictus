// shader.hpp: Loads shaders into opengl

#ifndef IS_SHADER_H_
#define IS_SHADER_H_

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "os.hpp"

namespace is {

class Shader {
public:
    Shader( std::string name, std::string vert, std::string frag );
    ~Shader();
    unsigned int getProgram();
    void         bind();
    void         setParameter( std::string name, int foo );
    void         setParameter( std::string name, glm::mat4 foo );
private:
    unsigned int getUniformLocation( std::string );
    int compile( unsigned int shader );
    int link( unsigned int vert, unsigned int frag );
    unsigned int m_program;
    std::string m_name;
};

};

#endif // IS_SHADER_H_
