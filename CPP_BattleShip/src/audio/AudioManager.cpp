#include "AudioManager.h"

AudioManager::AudioManager()
{
    InitAudioDevice();
    LoadSounds();
}

AudioManager::~AudioManager()
{
    StopMusic();
    UnloadSounds();
    CloseAudioDevice();
}

void AudioManager::LoadSounds()
{
    m_menuMusic = LoadMusicStream("assets/menu_bgm.mp3");
    m_gameMusic = LoadMusicStream("assets/game_bgm.mp3");
    m_endMusic  = LoadMusicStream("assets/endscreen.mp3");
    
    
    m_menuMusic.looping = true;
    m_gameMusic.looping = true;
    m_endMusic.looping  = true;

    m_hitSound  = LoadSound("assets/hit.mp3");
    m_missSound = LoadSound("assets/miss.mp3");
}

void AudioManager::UnloadSounds() const
{
    UnloadMusicStream(m_menuMusic);
    UnloadMusicStream(m_gameMusic);
    UnloadMusicStream(m_endMusic);

    UnloadSound(m_hitSound);
    UnloadSound(m_missSound);
}

void AudioManager::PlayMenuMusic()
{
    if (m_pCurrentMusic == &m_menuMusic)
        return;

    StopMusic();
    m_pCurrentMusic = &m_menuMusic;
    PlayMusicStream(*m_pCurrentMusic);
}

void AudioManager::PlayGameMusic()
{
    if (m_pCurrentMusic == &m_gameMusic)
        return;

    StopMusic();
    m_pCurrentMusic = &m_gameMusic;
    PlayMusicStream(*m_pCurrentMusic);
}

void AudioManager::PlayEndMusic()
{
    if (m_pCurrentMusic == &m_endMusic)
        return;

    StopMusic();
    m_pCurrentMusic = &m_endMusic;
    PlayMusicStream(*m_pCurrentMusic);
}

void AudioManager::StopMusic()
{
    if (m_pCurrentMusic != nullptr)
    {
        StopMusicStream(*m_pCurrentMusic);
        m_pCurrentMusic = nullptr;
    }
}

void AudioManager::UpdateMusic()
{
    if (m_pCurrentMusic != nullptr)
        UpdateMusicStream(*m_pCurrentMusic);
    
    if (m_pCurrentMusic == &m_menuMusic)
        SetMusicVolume(*m_pCurrentMusic, .025f);
    else if (m_pCurrentMusic == &m_gameMusic)
        SetMusicVolume(*m_pCurrentMusic, .4f);
    else if (m_pCurrentMusic == &m_endMusic)
        SetMusicVolume(*m_pCurrentMusic, .2f);
}

void AudioManager::PlayHit()
{
    SetSoundPitch(m_hitSound, 1.0f);
    SetSoundVolume(m_hitSound, 1.0f);
    PlaySound(m_hitSound);
}

void AudioManager::PlayMiss()
{
    PlaySound(m_missSound);
}

void AudioManager::PlaySunk()
{
    // Same sound as hit but lower pitch and higher volume — more imposing
    SetSoundPitch(m_hitSound, 0.4f);
    SetSoundVolume(m_hitSound, 2.f);
    PlaySound(m_hitSound);
}