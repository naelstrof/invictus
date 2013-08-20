// sound.hpp: Sound playable

#ifndef IS_SOUND_H_
#define IS_SOUND_H_

#include <glm/glm.hpp>
#include "audio.hpp"

namespace is {

class Audio;
class SoundBuffer;

class Sound {
public:
    Sound( std::string name );
    ~Sound();
    is::SoundBuffer* m_soundbuffer;
    unsigned int m_id;
    void play();
    void pause();
    void stop();
    void setPitch( float pitch );
    float getPitch();
    void setPos( glm::vec3 pos );
    glm::vec3 getPos();
    void setVel( glm::vec3 vel );
    glm::vec3 getVel();
    void loop( bool looping );
    int m_luaReference;
private:
    float     m_pitch;
    glm::vec3 m_vel;
    glm::vec3 m_pos;
};

};

#endif
