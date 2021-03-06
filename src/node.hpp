// node.hpp: Base object for inheritance.

#ifndef IS_NODE_H_
#define IS_NODE_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "os.hpp"
#include "lua.hpp"

#define PI 3.14159265359

namespace is {

class Node {
public:
                            Node();
    virtual                 ~Node();
    // This is important to make sure nodes properly clean themselves up.
    virtual void            remove();
    // String identifier
    virtual std::string     type();
    virtual void            tick( float dt );
    virtual void            draw();
    virtual bool            visible();

    glm::vec4               getColor();
    void                    setColor( glm::vec4 color );
    void                    setColor( float r, float g, float b, float a );

    glm::vec3               getPos();
    void                    setPos( glm::vec3 pos );
    void                    setPos( float x, float y, float z );

    glm::vec3               getAng();
    void                    setAng( glm::vec3 ang );
    void                    setAng( float y, float p, float r );

    glm::vec3               getScale();
    virtual void            setScale( glm::vec3 scale );
    virtual void            setScale( float w, float h, float d );

    is::Node*               getRoot();
    is::Node*               getParent();
    void                    setParent( is::Node* node );

    std::vector<is::Node*>  m_children;
    void                    removeChild( is::Node* node );
    void                    addChild( is::Node* node );

    void                    setMatrixChanged( bool changed );

    glm::mat4               getModelMatrix();
    glm::mat4               getScalelessMatrix();
    float                   m_depth;
    float                   m_hullsize;
    int                     m_luaReference;
    bool                    m_matrixChanged;
    is::Node*               m_parent;
    glm::mat4               m_matrix;
    glm::mat4               m_noscaleMatrix;
    glm::vec3               m_position;
    glm::vec3               m_angle;
    glm::vec3               m_scale;
    glm::vec4               m_color;
};

};

#endif // IS_NODE_H_
