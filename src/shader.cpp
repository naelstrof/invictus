#include "shader.hpp"

is::Shader::Shader( std::string name, std::string vert, std::string frag ) {
    m_good = false;
    // Create the program to link to.
    m_program = glCreateProgram();

    const char* vertsrc = vert.c_str();
    const char* fragsrc = frag.c_str();

    // Compile both shaders.
    unsigned int vertShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertShader, 1, &vertsrc , NULL );
    int err = compile( vertShader );

    unsigned int fragShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragShader, 1, &fragsrc, NULL );
    err += compile( fragShader );

    if ( err ) {
        os->printf( "ERR Failed to compile shader %.\n", name );
        glDeleteShader( vertShader );
        glDeleteShader( fragShader );
        return;
    }

    // Then attempt to link them to this shader.
    err = link( vertShader, fragShader );
    if ( err ) {
        os->printf( "ERR Failed to link shader %.\n", name );
        glDeleteShader( vertShader );
        glDeleteShader( fragShader );
        return;
    }

    // Clean up :)
    glDeleteShader( vertShader );
    glDeleteShader( fragShader );
    m_good = true;
}

is::Shader::~Shader() {
    glDeleteProgram( m_program );
}

unsigned int is::Shader::getProgram() {
    return m_program;
}

void is::Shader::bind() {
    glUseProgram( m_program );
}

int is::Shader::compile( unsigned int shader ) {
    glCompileShader( shader );

    // Compiling the shader is the easy part, all this junk down here is for printing the error it might generate.
    int result;
    int logLength;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &result );
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
    if ( result == GL_FALSE ) {
        char* errormsg = new char[ logLength ];
        glGetShaderInfoLog( shader, logLength, NULL, errormsg );
        os->printf( "%\n", errormsg );
        delete[] errormsg;
        return 1;
    }
    return 0;
}

int is::Shader::link( unsigned int vertshader, unsigned int fragshader ) {
    glAttachShader( m_program, vertshader );
    glAttachShader( m_program, fragshader );
    glLinkProgram( m_program );

    // Linking the shader is the easy part, all this junk down here is for printing the error it might generate.
    int result;
    int logLength;
    glGetProgramiv( m_program, GL_LINK_STATUS, &result);
    glGetProgramiv( m_program, GL_INFO_LOG_LENGTH, &logLength);
    if ( result == GL_FALSE ) {
        char* errormsg = new char[ logLength ];
        glGetProgramInfoLog( m_program, logLength, NULL, errormsg );
        os->printf( "%\n", errormsg );
        delete[] errormsg;
        return 1;
    }
    return 0;
}

unsigned int is::Shader::getUniformLocation( std::string name ) {
    if ( !m_good ) {
        return 0;
    }
    glUseProgram( m_program );
    return glGetUniformLocation( m_program, name.c_str() );
}

void is::Shader::setParameter( std::string name, int foo ) {
    if ( !m_good ) {
        return;
    }
    glUniform1i( getUniformLocation( name ), foo );
}

void is::Shader::setParameter( std::string name, glm::mat4 foo ) {
    if ( !m_good ) {
        return;
    }
    glUniformMatrix4fv( getUniformLocation( name ), 1, GL_FALSE, glm::value_ptr( foo ) );
}

void is::Shader::setParameter( std::string name, glm::vec4 foo ) {
    if ( !m_good ) {
        return;
    }
    glUniform4f( getUniformLocation( name ), foo.x, foo.y, foo.z, foo.w );
}
