#include "model.hpp"

is::Model* models = new is::Model();

is::Model::Model() {
}

is::Model::~Model() {
    // No need to delete any scenes, the importer deconstructer takes care of it.
    //for ( unsigned int i=0; i<m_scenes.size(); i++ ) {
        //delete m_scenes[i];
    //}
}

int is::Model::init() {
    lua->doFolder( "data/models" );
    return 0;
}

aiScene* is::Model::get( std::string modelname ) {
    for ( unsigned int i=0; i<m_modelnames.size(); i++ ) {
        if ( modelname == m_modelnames[i] ) {
            return m_scenes[i];
        }
    }
    os->printf( "ERR Couldn't find model %!\n", modelname );
    // TODO: Return a default model here so we don't crash.
    return NULL;
}

void is::Model::addModel( std::string modelname, std::string dir ) {
    is::File::Read file( dir );

    char* data = new char[ file.size() ];
    file.read( data, file.size() );

    aiScene* scene = (aiScene*)m_importer.ReadFileFromMemory( data, file.size(),
                                                    aiProcess_CalcTangentSpace       |
                                                    aiProcess_Triangulate            |
                                                    aiProcess_JoinIdenticalVertices  |
                                                    aiProcess_SortByPType );
    if ( !scene ) {
        os->printf( "ERR Error loading model: %. With reason: %\n", modelname, m_importer.GetErrorString() );
        return;
    }

    m_scenes.push_back( scene );
    m_modelnames.push_back( modelname );
    os->printf( "INF Loaded model %.\n", modelname );
}
