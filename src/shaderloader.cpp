#include "shaderloader.hpp"

is::ShaderLoader* shaders = new is::ShaderLoader();

is::ShaderLoader::ShaderLoader() {

}

is::ShaderLoader::~ShaderLoader() {
    for ( unsigned int i=0; i<m_shaders.size(); i++ ) {
        delete m_shaders[i];
    }
}

int is::ShaderLoader::init() {
    lua->doFolder( "data/shaders" );
    return 0;
}

is::Shader* is::ShaderLoader::get( std::string shadername ) {
    for ( unsigned int i=0; i<m_shaders.size(); i++ ) {
        if ( shadername == m_shadernames[i] ) {
            return m_shaders[i];
        }
    }
    os->printf( "ERR Couldn't find shader %!\n", shadername );
    // TODO: Return a default shader here so we don't crash.
    return NULL;
}

void is::ShaderLoader::addShader( std::string name, std::string vertdir, std::string fragdir ) {
    is::File::Read vert( vertdir );
    is::File::Read frag( fragdir );

    char* vertdata = new char[ vert.size() + 1 ];
    vert.read( vertdata, vert.size() );
    vertdata[ vert.size() ] = '\0';

    char* fragdata = new char[ frag.size() + 1 ];
    frag.read( fragdata, frag.size() );
    fragdata[ frag.size() ] = '\0';

    is::Shader* shader = new is::Shader( name, vertdata, fragdata );

    delete[] fragdata;
    delete[] vertdata;

    m_shaders.push_back( shader );
    m_shadernames.push_back( name );
    os->printf( "INF Loaded shader %.\n", name );
}
