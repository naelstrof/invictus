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
    Shader( std::string name, std::string vert, std::string frag, std::string shadertype = "other" );
    ~Shader();
    unsigned int    getProgram();
    void            bind();
    void            unbind();
    void            setParameter( std::string name, int foo );
    void            setParameter( std::string name, float foo );
    void            setParameter( std::string name, glm::mat4 foo );
    void            setParameter( std::string name, glm::vec4 foo );
    void            setParameter( std::string name, glm::vec2 foo );
    void            setAttribute( std::string name, unsigned int buffer, unsigned int stepsize );
    int             m_type;
    std::string     m_name;
private:
    std::vector<unsigned int>   m_activeattribs;
    bool                        m_good;
    unsigned int                getUniformLocation( std::string );
    int                         compile( unsigned int shader );
    int                         link( unsigned int vert, unsigned int frag );
    unsigned int                m_program;
};

};

#endif // IS_SHADER_H_
