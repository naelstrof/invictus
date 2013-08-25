#include "dropdown.hpp"

is::Dropdown::Dropdown( std::string texturename, float borderSize ) {
    m_stretched = 0;
    m_selectedElement = 0;
    m_active = false;
    m_pressed = false;
    m_released = false;
    m_border = borderSize;
    m_changed = true;
    m_texture = textures->get( texturename );
    setScale( glm::vec3( m_texture->getWidth(), m_texture->getHeight(), 1 ) );
    glGenBuffers( 2, m_buffers);
    m_shader = shaders->get( "gui" );
    m_luaChangeFunction = LUA_NOREF;
    m_text = new is::Text( "nil" );
    m_text->setParent( this );
}

is::Dropdown::~Dropdown() {
    glDeleteBuffers( 2, m_buffers );
    delete m_texture;
    delete m_text;
}

void is::Dropdown::remove() {
    delete (is::Dropdown*)this;
}

unsigned int is::Dropdown::addRow( std::string row ) {
    m_elements.push_back( row );
    if ( !m_active ) {
        if ( m_selectedElement < m_elements.size() ) {
            m_text->setText( m_elements.at( m_selectedElement ) );
        }
    } else {
        std::string temp;
        for ( unsigned int i=0; i<m_elements.size(); i++ ) {
            temp += m_elements.at(i) + "\n";
        }
        m_text->setText( temp );
    }
    glm::vec3 scale = getScale();
    scale.x = m_text->m_width;
    setScale( scale );
    return m_elements.size()-1;
}

std::string is::Dropdown::type() {
    return "dropdown";
}

void is::Dropdown::generateBuffers() {
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

bool is::Dropdown::intersects( glm::vec3 pos ) {
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

void is::Dropdown::tick( float dt ) {
    // Make sure our texture is animating properly.
    m_texture->tick( dt );
    // Since our text isn't part of the gui, we tick and draw it ourselves.
    m_text->tick( dt );

    // Make sure we can contain the whole text
    glm::vec3 scale = getScale();
    scale.x = m_text->m_width;
    setScale( scale );

    if ( !m_active ) {
        play( "idle" );
        // If we're still stretched from being active, unstretch!
        if ( m_stretched > 0 ) {
            glm::vec3 scale = getScale();
            scale.y = m_text->m_size;
            setScale( scale );
            m_stretched = 0;
            // If we aren't active, set the text to the selected element. ( Or leave it at nil if we don't have any elements to display. )
            if ( m_selectedElement < m_elements.size() ) {
                m_text->setText( m_elements.at( m_selectedElement ) );
            }
        }
        m_text->setPos( 0, 0, 0 );
        // Now we check if the mouse is interacting, and if so play certain animations.
        if ( intersects( mouse->getPos() ) ) {
            if ( mouse->isDown( is::Mouse::Left ) ) {
                m_released = false;
                m_pressed = true;
                play( "pressed" );
            } else {
                if ( m_pressed && !m_released ) {
                    m_released = true;
                }
                m_pressed = false;
                play( "active" );
            }
        } else {
            m_pressed = false;
            m_released = false;
        }

        // Check if the mouse released on the button, if so switch to active state.
        if ( m_released ) {
            m_active = true;
            m_released = false;
        }
    } else {
        play( "idle" );
        m_text->setPos( 0, getScale().y/2.f - m_text->m_size/2.f, 0 );
        // If we are active and not stretched to fit all the text, stretch!
        if ( m_stretched == 0 ) {
            m_stretched = m_elements.size();
            glm::vec3 scale = getScale();
            scale.y = m_stretched*m_text->m_size;
            setScale( scale );
            // also set the text to all of the elements.
            std::string temp;
            for ( unsigned int i=0; i<m_elements.size(); i++ ) {
                temp += m_elements.at(i) + "\n";
            }
            m_text->setText( temp );
        }
        // Now we check if the mouse is interacting, and if so play certain animations.
        if ( mouse->isDown( is::Mouse::Left ) ) {
            m_released = false;
            m_pressed = true;
            if ( intersects( mouse->getPos() ) ) {
                play( "pressed" );
            }
        } else {
            if ( m_pressed && !m_released ) {
                m_released = true;
            }
            m_pressed = false;
            if ( intersects( mouse->getPos() ) ) {
                play( "active" );
            }
        }
        // Check if the mouse released on us, if so switch to unactive state.
        if ( m_released ) {
            if ( intersects( mouse->getPos() ) ) {
                glm::vec3 startpos = getPos() + getScale().y/2.f;
                float test = (startpos - mouse->getPos()).y;
                test /= m_text->m_size;
                test = floor(test);
                if ( test >= 0 && test < m_elements.size() ) {
                    m_selectedElement = test;
                    lua_State* l = lua->m_l;
                    lua_rawgeti( l, LUA_REGISTRYINDEX, m_luaChangeFunction );
                    if ( !lua_isnil( l, -1 ) ) {
                        lua_rawgeti( l, LUA_REGISTRYINDEX, states->getCurrentState()->m_luaStateReference );
                        lua_pushnumber( l, test );
                        lua_pushstring( l, m_elements.at( m_selectedElement ).c_str() );
                        lua->call( l, 3, 0 );
                    } else {
                        lua_pop( l, 1 );
                    }
                }
            }
            m_active = false;
            m_released = false;
        }
    }
}

void is::Dropdown::draw() {
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

    // Since our text isn't part of the gui, we tick and draw it ourselves.
    if ( m_text->visible() ) {
        m_text->draw();
    }
}

void is::Dropdown::play( std::string name ) {
    m_texture->play( name );
}

bool is::Dropdown::visible() {
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

void is::Dropdown::setScale( glm::vec3 scale ) {
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

void is::Dropdown::setScale( float w, float h, float d ) {
    if ( m_scale == glm::vec3( w, h, d ) ) {
        return;
    }
    if ( w > m_border*2 ) {
        m_scale.x = w;
    }
    if ( h > m_border*2 ) {
        m_scale.y = h;
        //m_text->setSize( h );
    }
    m_scale.z = d;
    for( unsigned int i=0;i<m_children.size();i++ ) {
        m_children[i]->m_matrixChanged = true;
    }
    m_matrixChanged = true;
    m_changed = true;
}

void is::Dropdown::selectElement( unsigned int element ) {
    if ( m_selectedElement == element ) {
        return;
    }
    if ( element < m_elements.size() ) {
        m_selectedElement = element;
    }
    if ( !m_active ) {
        if ( m_selectedElement < m_elements.size() ) {
            m_text->setText( m_elements.at( m_selectedElement ) );
        }
    } else {
        std::string temp;
        for ( unsigned int i=0; i<m_elements.size(); i++ ) {
            temp += m_elements.at(i) + "\n";
        }
        m_text->setText( temp );
    }
    glm::vec3 scale = getScale();
    scale.x = m_text->m_width;
    setScale( scale );
}
