#include "framebuffer.hpp"

is::Framebuffer::Framebuffer() {
    generatedBuffers = false;
    m_width = 0;
    m_height = 0;
    m_flags = (is::Framebuffer::buffers)0;
}

is::Framebuffer::Framebuffer( unsigned int width, unsigned int height, unsigned char flags ) {
    generatedBuffers = false;
    m_width = width;
    m_height = height;
    m_flags = flags;
    if ( m_flags & stencil && m_flags & depth ) {
        os->printf( "WRN Most hardware doesn't support using a FBO with a depth and stencil attached! Condensing them to one render buffer..." );
    }
    if ( GLEW_VERSION_3_0 ) {
        glGenFramebuffers( 1, &m_frame );
        glBindFramebuffer( GL_FRAMEBUFFER, m_frame );
        if ( m_flags & color ) {
            glGenTextures( 1, &m_texture );
            glBindTexture( GL_TEXTURE_2D, m_texture );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0 );
        }

        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glGenRenderbuffers( 1, &m_depth );
            glBindRenderbuffer( GL_RENDERBUFFER, m_depth );
            glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth );
        }

        //This is the only way I could get a proper stencil buffer, by making a GL_DEPTH_STENCIL attachment to both points.
        if ( m_flags & stencil ) {
            glGenRenderbuffers( 1, &m_stencil );
            glBindRenderbuffer( GL_RENDERBUFFER, m_stencil );
            glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_STENCIL, m_width, m_height );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_stencil );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencil );
        }
        check();
        //Make sure we unbind when we're done.
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    } else if ( GLEW_EXT_framebuffer_object ) {
        glGenFramebuffersEXT( 1, &m_frame );
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frame );

        if ( m_flags & color ) {
            glGenTextures( 1, &m_texture );
            glBindTexture( GL_TEXTURE_2D, m_texture );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texture, 0 );
        }

        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glGenRenderbuffersEXT( 1, &m_depth );
            glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depth );
            glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depth );
        }

        //This is the only way I could get a proper stencil buffer, by making a GL_DEPTH_STENCIL attachment to both points.
        if ( m_flags & stencil ) {
            glGenRenderbuffersEXT( 1, &m_stencil );
            glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_stencil );
            glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL_EXT, m_width, m_height );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_stencil );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_stencil );
        }
        check();
        //Make sure we unbind when we're done.
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    } else {
        os->printf( "ERR Failed to create framebuffer! You need OpenGL 3.0 or the GL_EXT_framebuffer_object extension!\n" );
    }
}

is::Framebuffer::~Framebuffer() {
    if ( GLEW_VERSION_3_0 ) {
        glDeleteFramebuffers( 1, &m_frame );
        if ( m_flags & color ) {
            glDeleteTextures( 1, &m_texture );
        }
        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glDeleteRenderbuffers( 1, &m_depth );
        }
        if ( m_flags & stencil ) {
            glDeleteRenderbuffers( 1, &m_stencil );
        }
    } else if ( GLEW_EXT_framebuffer_object ) {
        glDeleteFramebuffersEXT( 1, &m_frame );
        if ( m_flags & color ) {
            glDeleteTextures( 1, &m_texture );
        }
        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glDeleteRenderbuffersEXT( 1, &m_depth );
        }
        if ( m_flags & stencil ) {
            glDeleteRenderbuffersEXT( 1, &m_stencil );
        }
    }
}

void is::Framebuffer::bind() {
    if ( GLEW_VERSION_3_0 ) {
        glBindFramebuffer( GL_FRAMEBUFFER, m_frame );
    } else if ( GLEW_EXT_framebuffer_object ) {
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frame );
    }
}

void is::Framebuffer::unbind() {
    if ( GLEW_VERSION_3_0 ) {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    } else if ( GLEW_EXT_framebuffer_object ) {
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    }
}

void is::Framebuffer::createFromTexture( unsigned int texture ) {
    // First make sure we're empty.
    if ( GLEW_VERSION_3_0 ) {
        glDeleteFramebuffers( 1, &m_frame );
        if ( m_flags & color ) {
            glDeleteTextures( 1, &m_texture );
        }
        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glDeleteRenderbuffers( 1, &m_depth );
        }
        if ( m_flags & stencil ) {
            glDeleteRenderbuffers( 1, &m_stencil );
        }
    } else if ( GLEW_EXT_framebuffer_object ) {
        glDeleteFramebuffersEXT( 1, &m_frame );
        if ( m_flags & color ) {
            glDeleteTextures( 1, &m_texture );
        }
        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glDeleteRenderbuffersEXT( 1, &m_depth );
        }
        if ( m_flags & stencil ) {
            glDeleteRenderbuffersEXT( 1, &m_stencil );
        }
    }
    // Set flags to 0 so we don't delete the texture on deconstruction.
    m_flags = 0;
    m_texture = texture;
    if ( GLEW_VERSION_3_0 ) {
        glGenFramebuffers( 1, &m_frame );
        glBindFramebuffer( GL_FRAMEBUFFER, m_frame );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0 );
    } else if ( GLEW_EXT_framebuffer_object ) {
        glGenFramebuffersEXT( 1, &m_frame );
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frame );
        glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texture, 0 );
    }
    check();
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void is::Framebuffer::create( unsigned int width, unsigned int height, unsigned char flags ) {
    if ( m_width == width && m_height == height && m_flags == flags ) {
        return;
    }
    if ( GLEW_VERSION_3_0 ) {
        glDeleteFramebuffers( 1, &m_frame );
        if ( m_flags & color ) {
            glDeleteTextures( 1, &m_texture );
        }
        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glDeleteRenderbuffers( 1, &m_depth );
        }
        if ( m_flags & stencil ) {
            glDeleteRenderbuffers( 1, &m_stencil );
        }
    } else if ( GLEW_EXT_framebuffer_object ) {
        glDeleteFramebuffersEXT( 1, &m_frame );
        if ( m_flags & color ) {
            glDeleteTextures( 1, &m_texture );
        }
        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glDeleteRenderbuffersEXT( 1, &m_depth );
        }
        if ( m_flags & stencil ) {
            glDeleteRenderbuffersEXT( 1, &m_stencil );
        }
    }
    m_flags = flags;
    m_width = width;
    m_height = height;
    if ( GLEW_VERSION_3_0 ) {
        glGenFramebuffers( 1, &m_frame );
        glBindFramebuffer( GL_FRAMEBUFFER, m_frame );

        if ( m_flags & color ) {
            glGenTextures( 1, &m_texture );
            glBindTexture( GL_TEXTURE_2D, m_texture );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0 );
        }

        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glGenRenderbuffers( 1, &m_depth );
            glBindRenderbuffer( GL_RENDERBUFFER, m_depth );
            glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth );
        }

        //This is the only way I could get a proper stencil buffer, by making a GL_DEPTH_STENCIL attachment to both points.
        if ( m_flags & stencil ) {
            glGenRenderbuffers( 1, &m_stencil );
            glBindRenderbuffer( GL_RENDERBUFFER, m_stencil );
            glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_STENCIL, m_width, m_height );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_stencil );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencil );
        }
        //Make sure we unbind when we're done.
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    } else if ( GLEW_EXT_framebuffer_object ) {
        glGenFramebuffersEXT( 1, &m_frame );
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frame );

        if ( m_flags & color ) {
            glGenTextures( 1, &m_texture );
            glBindTexture( GL_TEXTURE_2D, m_texture );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texture, 0 );
        }

        if ( m_flags & depth && !( m_flags & stencil ) ) {
            glGenRenderbuffersEXT( 1, &m_depth );
            glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depth );
            glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depth );
        }

        //This is the only way I could get a proper stencil buffer, by making a GL_DEPTH_STENCIL attachment to both points.
        if ( m_flags & stencil ) {
            glGenRenderbuffersEXT( 1, &m_stencil );
            glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_stencil );
            glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL_EXT, m_width, m_height );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_stencil );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_stencil );
        }
        //Make sure we unbind when we're done.
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    }
}

void is::Framebuffer::check()
{
    if ( GLEW_VERSION_3_0 ) {
        glBindFramebuffer( GL_FRAMEBUFFER, m_frame );
        unsigned int err = glCheckFramebufferStatus( GL_FRAMEBUFFER );
        if ( err == GL_FRAMEBUFFER_COMPLETE ) {
            return;
        }
        os->printf( "ERR Framebuffer failed a check procedure for reason: " );
        switch ( err ) {
            case GL_FRAMEBUFFER_UNDEFINED: {
                os->printf( "GL_FRAMEBUFFER_UNDEFINED\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n" );
                break;
            }
            case GL_FRAMEBUFFER_UNSUPPORTED: {
                os->printf( "GL_FRAMEBUFFER_UNSUPPORTED\n" );
                break;
            }
            default: {
                os->printf( "UNKNOWN\n" );
                break;
            }
        }
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    } else if ( GLEW_EXT_framebuffer_object ) {
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frame );
        unsigned int err = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
        if ( err == GL_FRAMEBUFFER_COMPLETE_EXT ) {
            return;
        }
        switch ( err ) {
            /* Apparently doesn't exist as an extension.
               case GL_FRAMEBUFFER_UNDEFINED_EXT: {
                os->printf( "GL_FRAMEBUFFER_UNDEFINED_EXT\n" );
                break;
            }*/
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT\n" );
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT: {
                os->printf( "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT\n" );
                break;
            }
            case GL_FRAMEBUFFER_UNSUPPORTED_EXT: {
                os->printf( "GL_FRAMEBUFFER_UNSUPPORTED_EXT\n" );
                break;
            }
            default: {
                os->printf( "Unknown framebuffer error!\n" );
                break;
            }
        }
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    }
}

void is::Framebuffer::generateBuffers() {
    if ( generatedBuffers ) {
        return;
    }
    std::vector<glm::vec2> verts;
    std::vector<glm::vec2> uvs;
    verts.push_back( glm::vec2( -1, -1 ) );
    verts.push_back( glm::vec2( 1, -1 ) );
    verts.push_back( glm::vec2( 1, 1 ) );
    verts.push_back( glm::vec2( -1, 1 ) );
    uvs.push_back( glm::vec2( 0, 0 ) );
    uvs.push_back( glm::vec2( 1, 0 ) );
    uvs.push_back( glm::vec2( 1, 1 ) );
    uvs.push_back( glm::vec2( 0, 1 ) );

    glGenBuffers( 2, m_buffers );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[0] );
    glBufferData( GL_ARRAY_BUFFER, verts.size() * sizeof( glm::vec2 ), &verts[0], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[1] );
    glBufferData( GL_ARRAY_BUFFER, uvs.size() * sizeof( glm::vec2 ), &uvs[0], GL_STATIC_DRAW );
    generatedBuffers = true;
}

void is::Framebuffer::clear( glm::vec4 clearcolor, unsigned char flags ) {
    // FIXME: This should auto-bind and unbind, but I was too lazy to find out how to push and pop gl states.
    unsigned int colorbit = 0;
    if ( flags & color ) {
        colorbit = colorbit | GL_COLOR_BUFFER_BIT;
    }
    if ( m_flags & depth && flags & depth ) {
        colorbit = colorbit | GL_DEPTH_BUFFER_BIT;
    }
    if ( m_flags & stencil && flags & stencil) {
        colorbit = colorbit | GL_STENCIL_BUFFER_BIT;
    }
    glClearColor( clearcolor.x, clearcolor.y, clearcolor.z, clearcolor.w );
    glClear( colorbit );
}

void is::Framebuffer::draw() {
    if ( !( m_flags & color ) ) {
        return;
    }

    generateBuffers();

    is::Shader* shader = shaders->get( "simple" );
    shader->bind();
    shader->setParameter( "texture", 0 );
    shader->setAttribute( "vertex", m_buffers[0], 2 );
    shader->setAttribute( "uv", m_buffers[1], 2 );

    glBindTexture( GL_TEXTURE_2D, m_texture );
    glEnable( GL_TEXTURE_2D );
    glDrawArrays( GL_QUADS, 0, 4 );
    glDisable( GL_TEXTURE_2D );

    shader->unbind();
}
