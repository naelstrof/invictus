// model.hpp: System that handles grabbing requested models and loading them.

#ifndef IS_MODEL_H_
#define IS_MODEL_H_

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "lua.hpp"

namespace is {

class Model {
private:
    Assimp::Importer            m_importer;
    std::vector<std::string>    m_modelnames;
    std::vector<aiScene*>       m_scenes;
public:
    Model();
    ~Model();
    int             init();
    aiScene*        get( std::string modelname );
    void            addModel( std::string modelname, std::string dir );
};

};

extern is::Model* models;

#endif // IS_MODEL_H_
