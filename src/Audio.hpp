#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SDL2/SDL_mixer.h>
#include <boost/filesystem.hpp>
#include <string>
#include <map>
namespace fs = boost::filesystem;

/// Automatically looks for music files at startup. Music is played constantly.
/// Sound effects can be played back by calling playSound
class Audio
{
public:
    Audio ();
    virtual ~Audio ();

    /// Advance to next song. Start from beginning if none are left in the list.
    virtual void nextSong();
    /// Pauses playback of music and sound effects
    virtual void pause();
    /// Starts playback of music and sound effects
    virtual void play();
    /// Plays the sound file with the given name. Aborts the currently playing sound.
    /// @return Whether the sound file was found.
    virtual bool playSound(const std::string& soundName);
    /// Sets the music volume (0-100)
    virtual void setMusicVolume(unsigned char volume);
    /// Sets the sound effect volume (0-100)
    virtual void setSoundEffectVolume(unsigned char volume);

private:
    Audio (const Audio& other);
    Audio& operator= (const Audio& other);

protected:
    fs::path mSoundPath;
    fs::path mSoundEffectPath;
    fs::path mMusicPath;

    unsigned char mMusicVolume;
    unsigned char mSoundEffectVolume;

    bool mPaused;

    std::map<std::string, Mix_Chunk*> mSoundEffects;
    std::map<std::string, Mix_Music*> mMusicTracks;
    std::map<std::string, Mix_Music*>::const_iterator mCurrentMusicTrack;
};

#endif /* end of include guard: AUDIO_HPP */

