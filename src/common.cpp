#include "common.hpp"

is::Common::Common() {
    m_running = false;
}

is::Common::~Common() {
    window->close();
}

is::Common* common = new is::Common();

int is::Common::init( int argc, char** argv ) {
    boost::nowide::args( argc, argv );

    // Parse game options.
    int width = 0;
    int height = 0;
    bool fullscreen = false;
    bool noborder = false;
    for ( int i=0; i<argc; i++ ) {
        std::string arg = argv[i];
        if ( arg == "-w" || arg == "--width" ) {
            width = atoi( argv[i+1] );
            i++;
            continue;
        }
        if ( arg == "-h" || arg == "--height" ) {
            height = atoi( argv[i+1] );
            i++;
            continue;
        }
        if ( arg == "--fullscreen" ) {
            fullscreen = true;
            continue;
        }
        if ( arg == "--windowed" ) {
            fullscreen = false;
            continue;
        }
        if ( arg == "--noborder" ) {
            noborder = true;
            continue;
        }
    }

    // Finally initialize the game.
    int err = filesystem->init( argv[0] );
    if ( err ) {
        os->printf( "ERR Failed initialize the filesystem, shutting down...\n" );
        return err;
    }

    err = lua->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize lua, shutting down...\n" );
        return err;
    }

    err = window->init();
    if ( err ) {
        os->printf( "ERR Failed to open window, shutting down...\n" );
        return err;
    }
    if ( fullscreen ) {
        window->setFullscreen( true );
    } else {
        window->setFullscreen( false );
    }
    if ( noborder ) {
        window->setNoBorder( true );
    }
    if ( width || height ) {
        window->setSize( width, height );
    }

    err = glewInit();
    if ( err != GLEW_OK ) {
        os->printf( "ERR %\n", glewGetErrorString( err ) );
        os->printf( "ERR Failed to initialize glew, shutting down...\n" );
        return err;
    }

    err = shaders->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize shaders, shutting down...\n" );
        return err;
    }

    err = textures->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize textures, shutting down...\n" );
        return err;
    }

    err = fonts->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize fonts, shutting down...\n" );
        return err;
    }

    err = models->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize models, shutting down...\n" );
        return err;
    }

    err = render->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize render engine, shutting down...\n" );
        return err;
    }

    err = states->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize state machine, shutting down...\n" );
        return err;
    }
    states->setState( "intro" );

    m_running = true;
    m_time.restart();
    return 0;
}

void is::Common::tick() {
    /*if ( mouse->isDown( is::Mouse::Left ) ) {
        glm::vec3 angle = render->m_camera->getAng();
        glm::vec3 forward = glm::vec3( sin( angle.x ),
                                       cos( angle.x )*cos( angle.y ),
                                       cos( angle.x )*sin( angle.y ) );
        is::Icon* dothan = new is::Icon( "dothan" );
        dothan->setScale( glm::vec3( 64, 64, 1 ) );
        dothan->setPos( forward*300.f );
        dothan->setAng( glm::vec3( PI+angle.x, 0, 0 ) );
        dothan->setColor( glm::vec4( float(rand()%100)/100.f,
                                     float(rand()%100)/100.f,
                                     float(rand()%100)/100.f, 1.f ) );
        dothan->m_shader = shaders->get( "unlitGeneric" );
        world->addNode( dothan );
    }*/
    if ( keyboard->isDown( is::Keyboard::Escape ) || keyboard->isDown( is::Keyboard::Q ) ) {
        m_running = false;
        return;
    }
    if ( !window->isOpen() ) {
        m_running = false;
        return;
    }
    sf::Time dt = m_time.restart();
    if ( keyboard->isDown( is::Keyboard::A ) ) {
        render->m_camera->setAng( render->m_camera->getAng() - glm::vec3( dt.asSeconds(), 0, 0 ) );
    }
    if ( keyboard->isDown( is::Keyboard::D ) ) {
        render->m_camera->setAng( render->m_camera->getAng() + glm::vec3( dt.asSeconds(), 0, 0 ) );
    }
    states->tick( dt.asSeconds() );
    window->tick();
    world->tick( dt.asSeconds() );
    gui->tick( dt.asSeconds() );
    render->tick();
    render->draw();
}
