#include "Audio.hpp"

#include "common.h"
#include <sstream>

Audio::Audio()
    : mSoundPath(fs::current_path() / ".." / "res" / "sounds")
    , mSoundEffectPath(mSoundPath / "effects")
    , mMusicPath(mSoundPath / "music")
    , mPaused(false)
    , mCurrentMusicTrack(mMusicTracks.end())
{
    if (!fs::is_directory(mSoundPath)) {
        throw std::runtime_error("Invalid sound path: " + mSoundPath.string());
    }
    if (!fs::is_directory(mSoundEffectPath)) {
        throw std::runtime_error("Invalid sound effect path: " + mSoundEffectPath.string());
    }
    if (!fs::is_directory(mMusicPath)) {
        throw std::runtime_error("Invalid music path: " + mMusicPath.string());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        std::stringstream msg ("Could not initialize audio: ");
        msg << SDL_GetError();
        throw std::runtime_error(msg.str());
    }
    int init_flags = MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MP3;
    if ((Mix_Init(init_flags) & init_flags) != init_flags) {
        LogWarning("Could not load all audio codecs: " << Mix_GetError());
    }

    Mix_AllocateChannels(16);

    const fs::directory_iterator end;

    // Only load names of sound effects
    for (fs::directory_iterator iter(mSoundEffectPath); iter != end; ++iter) {
        if (fs::is_regular_file(iter->status())) {
            mSoundEffects[iter->path().filename().string()] = nullptr;
        }
    }

    for (fs::directory_iterator iter(mMusicPath); iter != end; ++iter) {
        if (fs::is_regular_file(iter->status())) {
            Mix_Music* music = Mix_LoadMUS(iter->path().c_str());
            if (music) {
                mMusicTracks[iter->path().filename().string()] = music;
            } else {
                LogWarning("Error loading music file \"" << iter->path().string() << "\" : " << Mix_GetError());
            }
        }
    }
    mCurrentMusicTrack = mMusicTracks.begin();
}

Audio::~Audio()
{
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    for (auto& entry : mMusicTracks) {
        if (entry.second) {
            Mix_FreeMusic(entry.second);
            entry.second = nullptr;
        }
    }
    for (auto& entry : mSoundEffects) {
        if (entry.second) {
            Mix_FreeChunk(entry.second);
            entry.second = nullptr;
        }
    }
    Mix_Quit();
    Mix_CloseAudio();
}

void Audio::nextSong()
{
    if (mCurrentMusicTrack != mMusicTracks.end()) {
        mCurrentMusicTrack++;
    } else {
        mCurrentMusicTrack = mMusicTracks.begin();
    }
}

void Audio::pause()
{
    mPaused = true;
    Mix_PauseMusic();
    Mix_Pause(-1);
}

void Audio::play()
{
    mPaused = false;
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    } else if(!Mix_PlayingMusic()) {
        if (!mMusicTracks.empty() && mCurrentMusicTrack != mMusicTracks.end()) {
            if (Mix_PlayMusic(mCurrentMusicTrack->second, -1) == -1) {
                LogError("Cannot play music!");
            }
        }
    }
    Mix_Resume(-1);
}

bool Audio::playSound(const std::string& soundName)
{
    const auto& effectEntry = mSoundEffects.find(soundName);
    if (effectEntry == mSoundEffects.end()) {
        return false;
    }
    if (!effectEntry->second) {
        //load
        fs::path soundFilePath = mSoundEffectPath / soundName;
        if (!fs::is_regular_file(soundFilePath)) {
            LogError("Sound file not a valid file: " << soundFilePath.string());
            return false;
        }

        // All hail Lord WAV!
        effectEntry->second = Mix_LoadWAV(soundFilePath.c_str());
        // screw invalid sound files
        if (!effectEntry->second) {
            LogWarning("Error loading sound effect file \"" << soundFilePath.string() << "\" : " << Mix_GetError());
            return false;
        }
    }

    if (Mix_PlayChannel(-1, effectEntry->second, 0) == -1) {
        LogWarning("Error playing sound effect: \"" << effectEntry->first << "\" : " << Mix_GetError());
        return false;
    }

    return true;
}

void Audio::setMusicVolume(unsigned char volume)
{
    Mix_VolumeMusic(volume);
}

void Audio::setSoundEffectVolume(unsigned char volume)
{
    Mix_Volume(-1, volume);
}

