// audio.hpp: System that handles grabbing requested audio files to play.

#ifndef IS_AUDIO_H_
#define IS_AUDIO_H_

#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x
#define checkErrorSimple() checkError( __FILE__ ":" STRINGIFY( __LINE__ ) )

#include <AL/alure.h>

#include "os.hpp"
#include "lua.hpp"
#include "sound.hpp"

namespace is {

class Sound;

class SoundBuffer {
public:
    SoundBuffer( std::string name, std::string dir );
    ~SoundBuffer();
    int load();
    unsigned int m_id;
    std::string m_dir;
    bool m_loaded;
    std::string m_name;
};

class Audio {
private:
    std::vector<SoundBuffer*> m_soundbuffers;
    std::vector<Sound*> m_sounds;
public:
    Audio();
    ~Audio();
    int              init();
    void             checkError( std::string message = "");
    void             addSoundBuffer( std::string name, std::string dir );
    is::SoundBuffer* getSoundBuffer( std::string name );
    void             addSound( is::Sound* sound );
    void             removeSound( is::Sound* sound );
};

};

extern is::Audio* audio;

#endif // IS_AUDIO_H_
