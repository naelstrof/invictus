#include "audio.hpp"

is::Audio* audio = new is::Audio();

is::Audio::Audio() {
}

is::Audio::~Audio() {
    for ( unsigned int i=0; i<m_soundbuffers.size(); i++ ) {
        delete m_soundbuffers.at(i);
    }
    alureShutdownDevice();
}

int is::Audio::init() {
    // Grab available devices and print them.
    int deviceCount;
    const char** deviceNames = alureGetDeviceNames( true, &deviceCount );
    if ( !deviceCount ) {
        os->printf( "ERR OpenAL failed to find any usable devices for audio playback!\n" );
        alureFreeDeviceNames( deviceNames );
        checkError( "audio.cpp:19" );
        return 1;
    }
    os->printf( "INF OpenAL detected the following devices for audio:\n" );
    for ( int i=0; i<deviceCount; i++ ) {
        os->printf( "INF     %.\n", deviceNames[i] );
    }

    // Attempt to use any device starting from the first.
    int i = 0;
    while ( alureInitDevice( deviceNames[i], NULL ) == AL_FALSE && i < deviceCount ) {
        checkError( "audio.cpp:30" );
        i++;
    }
    if ( i == deviceCount ) {
        os->printf( "ERR OpenAL failed to find any usable devices for audio playback!\n" );
        alureFreeDeviceNames( deviceNames );
        return 1;
    }
    os->printf( "INF OpenAL: Using %.\n", deviceNames[i] );
    alureFreeDeviceNames( deviceNames );

    // Initialize the listener
    alListener3f( AL_POSITION, 0, 0, 0 );
    alListener3f( AL_VELOCITY, 0, 0, 0 );
    alListener3f( AL_ORIENTATION, 0, 0, -1 );
    checkError( "audio.cpp:45" );

    lua->doFolder( "data/sounds" );
    return 0;
}

void is::Audio::addSound( is::Sound* sound ) {
    m_sounds.push_back( sound );
}

void is::Audio::removeSound( is::Sound* sound ) {
    for ( unsigned int i=0; i<m_sounds.size(); i++ ) {
        if ( m_sounds.at(i) == sound ) {
            m_sounds.erase( m_sounds.begin() + i );
            return;
        }
    }
}

void is::Audio::addSoundBuffer( std::string name, std::string dir ) {
    m_soundbuffers.push_back( new is::SoundBuffer( name, dir ) );
}

is::SoundBuffer* is::Audio::getSoundBuffer( std::string name ) {
    for ( unsigned int i=0; i<m_soundbuffers.size(); i++ ) {
        if ( name == m_soundbuffers.at(i)->m_name ) {
            m_soundbuffers.at(i)->load();
            return m_soundbuffers.at(i);
        }
    }
    return NULL;
}

void is::Audio::checkError( std::string message ) {
    ALenum error = alGetError();
    while ( error != AL_NO_ERROR ) {
        switch( error ) {
            case AL_INVALID_NAME:       { os->printf( "ERR Alure %: invalid name parameter.\n", message ); break; }
            case AL_INVALID_ENUM:       { os->printf( "ERR Alure %: invalid parameter.\n", message ); break; }
            case AL_INVALID_VALUE:      { os->printf( "ERR Alure %: invalid enum parameter value.\n", message ); break; }
            case AL_INVALID_OPERATION:  { os->printf( "ERR Alure %: invalid operation.\n", message ); break; }
            case AL_OUT_OF_MEMORY:      { os->printf( "ERR Alure %: Unable to allocate more memory.\n", message ); break; }
            default:                    { os->printf( "ERR Alure %: Unkown error.\n", message ); break; }
        }
        error = alGetError();
    }
}

is::SoundBuffer::SoundBuffer( std::string name, std::string dir ) {
    m_name = name;
    m_dir = dir;
    // No need to generate buffers since alure does it for us.
    m_id = AL_NONE;
    m_loaded = false;
}

is::SoundBuffer::~SoundBuffer() {
    alDeleteBuffers( 1, &m_id );
}

int is::SoundBuffer::load() {
    is::File::Read file( m_dir );
    unsigned char* data = new unsigned char[ file.size() ];
    file.read( data, file.size() );
    m_id = alureCreateBufferFromMemory( data, file.size() );
    delete[] data;
    if ( m_id == AL_NONE ) {
        os->printf( "ERR Alure failed to open file % as a sound file!\n", m_dir );
        audio->checkError( "audio.cpp:85" );
        return 1;
    }
    m_loaded = true;
    return 0;
}
