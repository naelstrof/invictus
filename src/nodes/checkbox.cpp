#include "checkbox.hpp"

is::Checkbox::Checkbox( std::string texturename, float borderSize, bool checked ) {
    m_checked = checked;
    m_pressed = false;
    m_released = false;
    m_border = borderSize;
    m_changed = true;
    m_texture = textures->get( texturename );
    setScale( glm::vec3( m_texture->getWidth(), m_texture->getHeight(), 1 ) );
    glGenBuffers( 2, m_buffers);
    m_shader = shaders->get( "gui" );
    m_luaCheckFunction = LUA_NOREF;
    m_luaUncheckFunction = LUA_NOREF;
}

is::Checkbox::~Checkbox() {
    glDeleteBuffers( 2, m_buffers );
    delete m_texture;
}

void is::Checkbox::remove() {
    delete (is::Checkbox*)this;
}

std::string is::Checkbox::type() {
    return "checkbox";
}

void is::Checkbox::generateBuffers() {
    // We need to check to make sure we actually need to regenerate the buffers.
    if ( !m_changed ) {
        return;
    }

    std::vector<glm::vec2> verts;
    std::vector<glm::vec2> uvs;

    if ( m_border > 0 ) {
        float sy = m_border/getScale().y;
        float sx = m_border/getScale().x;
        float ux = m_border/m_texture->getWidth();
        float uy = m_border/m_texture->getHeight();
        //Top Left Corner
        verts.push_back(glm::vec2(-.5+sx,.5));
        uvs.push_back(glm::vec2(ux,0));
        verts.push_back(glm::vec2(-.5,.5));
        uvs.push_back(glm::vec2(0,0));
        verts.push_back(glm::vec2(-.5,.5-sy));
        uvs.push_back(glm::vec2(0,uy));
        verts.push_back(glm::vec2(-.5+sx,.5-sy));
        uvs.push_back(glm::vec2(ux,uy));
        //Top
        verts.push_back(glm::vec2(.5-sx,.5));
        uvs.push_back(glm::vec2(1-ux,0));
        verts.push_back(glm::vec2(-.5+sx,.5));
        uvs.push_back(glm::vec2(ux,0));
        verts.push_back(glm::vec2(-.5+sx,.5-sy));
        uvs.push_back(glm::vec2(ux,uy));
        verts.push_back(glm::vec2(.5-sx,.5-sy));
        uvs.push_back(glm::vec2(1-ux,uy));
        //Top Right Corner
        verts.push_back(glm::vec2(.5,.5));
        uvs.push_back(glm::vec2(1,0));
        verts.push_back(glm::vec2(.5-sx,.5));
        uvs.push_back(glm::vec2(1-ux,0));
        verts.push_back(glm::vec2(.5-sx,.5-sy));
        uvs.push_back(glm::vec2(1-ux,uy));
        verts.push_back(glm::vec2(.5,.5-sy));
        uvs.push_back(glm::vec2(1,uy));
        //Left
        verts.push_back(glm::vec2(-.5+sx,.5-sy));
        uvs.push_back(glm::vec2(ux,uy));
        verts.push_back(glm::vec2(-.5,.5-sy));
        uvs.push_back(glm::vec2(0,uy));
        verts.push_back(glm::vec2(-.5,-.5+sy));
        uvs.push_back(glm::vec2(0,1-uy));
        verts.push_back(glm::vec2(-.5+sx,-.5+sy));
        uvs.push_back(glm::vec2(ux,1-uy));
        //Middle
        verts.push_back(glm::vec2(.5-sx,.5-sy));
        uvs.push_back(glm::vec2(1-ux,uy));
        verts.push_back(glm::vec2(-.5+sx,.5-sy));
        uvs.push_back(glm::vec2(ux,uy));
        verts.push_back(glm::vec2(-.5+sx,-.5+sy));
        uvs.push_back(glm::vec2(ux,1-uy));
        verts.push_back(glm::vec2(.5-sx,-.5+sy));
        uvs.push_back(glm::vec2(1-ux,1-uy));
        //Right
        verts.push_back(glm::vec2(.5,.5-sy));
        uvs.push_back(glm::vec2(1,uy));
        verts.push_back(glm::vec2(.5-sx,.5-sy));
        uvs.push_back(glm::vec2(1-ux,uy));
        verts.push_back(glm::vec2(.5-sx,-.5+sy));
        uvs.push_back(glm::vec2(1-ux,1-uy));
        verts.push_back(glm::vec2(.5,-.5+sy));
        uvs.push_back(glm::vec2(1,1-uy));
        //Bottom Left Corner
        verts.push_back(glm::vec2(-.5+sx,-.5+sy));
        uvs.push_back(glm::vec2(ux,1-uy));
        verts.push_back(glm::vec2(-.5,-.5+sy));
        uvs.push_back(glm::vec2(0,1-uy));
        verts.push_back(glm::vec2(-.5,-.5));
        uvs.push_back(glm::vec2(0,1));
        verts.push_back(glm::vec2(-.5+sx,-.5));
        uvs.push_back(glm::vec2(ux,1));
        //Bottom
        verts.push_back(glm::vec2(.5-sx,-.5+sy));
        uvs.push_back(glm::vec2(1-ux,1-uy));
        verts.push_back(glm::vec2(-.5+sx,-.5+sy));
        uvs.push_back(glm::vec2(ux,1-uy));
        verts.push_back(glm::vec2(-.5+sx,-.5));
        uvs.push_back(glm::vec2(ux,1));
        verts.push_back(glm::vec2(.5-sx,-.5));
        uvs.push_back(glm::vec2(1-ux,1));
        //Bottom Right Corner
        verts.push_back(glm::vec2(.5,-.5+sy));
        uvs.push_back(glm::vec2(1,1-uy));
        verts.push_back(glm::vec2(.5-sx,-.5+sy));
        uvs.push_back(glm::vec2(1-ux,1-uy));
        verts.push_back(glm::vec2(.5-sx,-.5));
        uvs.push_back(glm::vec2(1-ux,1));
        verts.push_back(glm::vec2(.5,-.5));
        uvs.push_back(glm::vec2(1,1));
    } else {
        verts.push_back( glm::vec2( -0.5, -0.5 ) );
        verts.push_back( glm::vec2( 0.5, -0.5 ) );
        verts.push_back( glm::vec2( 0.5, 0.5 ) );
        verts.push_back( glm::vec2( -0.5, 0.5 ) );

        uvs.push_back( glm::vec2( 0, 1 ) );
        uvs.push_back( glm::vec2( 1, 1 ) );
        uvs.push_back( glm::vec2( 1, 0 ) );
        uvs.push_back( glm::vec2( 0, 0 ) );
    }

    // Send the buffers to opengl and clean up
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[0] );
    glBufferData( GL_ARRAY_BUFFER, verts.size() * sizeof( glm::vec2 ), &verts[0], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[1] );
    glBufferData( GL_ARRAY_BUFFER, uvs.size() * sizeof( glm::vec2 ), &uvs[0], GL_STATIC_DRAW );
    m_vertcount = verts.size();
    m_changed = false;
}

bool is::Checkbox::intersects( glm::vec3 pos ) {
    glm::vec3 p = getPos();
    glm::vec3 s = getScale()/2.f;
    if ( pos.x < p.x-s.x ) {
        return false;
    }
    if ( pos.x > p.x+s.x ) {
        return false;
    }
    if ( pos.y < p.y-s.y ) {
        return false;
    }
    if ( pos.y > p.y+s.y ) {
        return false;
    }
    return true;
}

void is::Checkbox::tick( float dt ) {
    // Make sure our texture is animating properly.
    m_texture->tick( dt );

    if ( intersects( mouse->getPos() ) ) {
        if ( mouse->isDown( is::Mouse::Left ) ) {
            m_released = false;
            m_pressed = true;
        } else {
            if ( !m_released && m_pressed ) {
                m_released = true;
                m_checked = !m_checked;
                if ( m_checked ) {
                    lua_State* l = lua->m_l;
                    lua_rawgeti( l, LUA_REGISTRYINDEX, m_luaCheckFunction );
                    if ( !lua_isnil( l, -1 ) ) {
                        lua_rawgeti( l, LUA_REGISTRYINDEX, states->getCurrentState()->m_luaStateReference );
                        lua->call( l, 1, 0 );
                    } else {
                        lua_pop( l, 1 );
                    }
                } else {
                    lua_State* l = lua->m_l;
                    lua_rawgeti( l, LUA_REGISTRYINDEX, m_luaUncheckFunction );
                    if ( !lua_isnil( l, -1 ) ) {
                        lua_rawgeti( l, LUA_REGISTRYINDEX, states->getCurrentState()->m_luaStateReference );
                        lua->call( l, 1, 0 );
                    } else {
                        lua_pop( l, 1 );
                    }
                }
            }
            m_pressed = false;
            if ( m_checked ) {
                play( "checkedActive" );
            } else {
                play( "uncheckedActive" );
            }
        }
    } else {
        m_released = false;
        m_pressed = false;
        if ( m_checked ) {
            play( "checkedIdle" );
        } else {
            play( "idle" );
        }
    }

}

void is::Checkbox::draw() {
    // Here we'll make sure we have a properly generated buffer.
    generateBuffers();

    m_shader->bind();
    m_shader->setParameter( "texture", 0 );
    m_shader->setParameter( "color", getColor() );
    m_shader->setParameter( "model", getModelMatrix() );
    m_shader->setAttribute( "vertex", m_buffers[0], 2 );
    m_shader->setAttribute( "uv", m_buffers[1], 2 );

    m_texture->bind();
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_TEXTURE_2D );
    glDrawArrays( GL_QUADS, 0, m_vertcount );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );

    m_shader->unbind();
}

void is::Checkbox::play( std::string name ) {
    m_texture->play( name );
}

bool is::Checkbox::visible() {
    // Always draw if part of the gui (due to the view frustum culling only working on orthographic objects)
    if ( m_shader->m_name == "gui" ) {
        return true;
    }
    // Otherwise test if we're inside the view frustum
    glm::vec3 size = getScale();
    m_hullsize = std::max( size.x, size.y )/2.f;

    is::Camera* cam = render->m_camera;
    //if ( cam->m_frustum->pointInFrustum( getPos(), &m_depth ) == is::Frustum::outside ) {
    if ( cam->m_frustum->sphereInFrustum( getPos(), m_hullsize, &m_depth ) == is::Frustum::outside ) {
        return false;
    }
    return true;
}

void is::Checkbox::setScale( glm::vec3 scale ) {
    if ( m_scale == scale ) {
        return;
    }
    if ( scale.x > m_border*2 ) {
        m_scale.x = scale.x;
    }
    if ( scale.y > m_border*2 ) {
        m_scale.y = scale.y;
    }
    m_scale.z = scale.z;
    for( unsigned int i=0;i<m_children.size();i++ ) {
        m_children[i]->m_matrixChanged = true;
    }
    m_matrixChanged = true;
    m_changed = true;
}

void is::Checkbox::setScale( float w, float h, float d ) {
    if ( m_scale == glm::vec3( w, h, d ) ) {
        return;
    }
    if ( w > m_border*2 ) {
        m_scale.x = w;
    }
    if ( h > m_border*2 ) {
        m_scale.y = h;
    }
    m_scale.z = d;
    for( unsigned int i=0;i<m_children.size();i++ ) {
        m_children[i]->m_matrixChanged = true;
    }
    m_matrixChanged = true;
    m_changed = true;
}
