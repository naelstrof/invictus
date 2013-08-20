#include "sound.hpp"

is::Sound::Sound( std::string name ) {
    m_pitch = 0;
    m_pos = glm::vec3( 0 );
    m_vel = glm::vec3( 0 );
    m_luaReference = LUA_NOREF;
    m_soundbuffer = audio->getSoundBuffer( name );
    alGenSources( 1, &m_id );
    alSourcef( m_id, AL_PITCH, 1 );
    alSourcef( m_id, AL_GAIN, 1 );
    alSource3f( m_id, AL_POSITION, 0, 0, 0 );
    alSource3f( m_id, AL_VELOCITY, 0, 0, 0 );
    alSourcei( m_id, AL_LOOPING, AL_FALSE );
    alSourcei( m_id, AL_BUFFER, m_soundbuffer->m_id );
    audio->addSound( this );
}

is::Sound::~Sound() {
    audio->removeSound( this );
    alDeleteSources( 1, &m_id );
}

void is::Sound::setPitch( float pitch ) {
    m_pitch = pitch;
    alSourcef( m_id, AL_PITCH, pitch );
}

float is::Sound::getPitch() {
    return m_pitch;
}

void is::Sound::play() {
    alSourcePlay( m_id );
}

void is::Sound::pause() {
    alSourcePause( m_id );
}

void is::Sound::stop() {
    alSourceStop( m_id );
}

void is::Sound::setPos( glm::vec3 pos ) {
    alSource3f( m_id, AL_POSITION, pos.x, pos.y, pos.z );
    m_pos = pos;
}

void is::Sound::setVel( glm::vec3 vel ) {
    alSource3f( m_id, AL_VELOCITY, vel.x, vel.y, vel.z );
    m_vel = vel;
}

glm::vec3 is::Sound::getVel() {
    return m_vel;
}

glm::vec3 is::Sound::getPos() {
    return m_pos;
}

void is::Sound::loop( bool looping ) {
    if ( looping ) {
        alSourcei( m_id, AL_LOOPING, AL_TRUE );
        return;
    }
    alSourcei( m_id, AL_LOOPING, AL_FALSE );
}
