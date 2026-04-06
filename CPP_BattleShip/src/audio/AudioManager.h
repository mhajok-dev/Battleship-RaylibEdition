/*****************************************************************************
+ Project: Battleship - Raylib Edition                                      +
 + File: AudioManager.h                                                      +
 +                                                                           +
 + Manages all game audio — music and sound effects.                         +
 + SRP: Only responsible for loading, playing and unloading audio.           +
 + RAII: All audio resources are unloaded in the destructor.                 +
 *****************************************************************************/

#pragma once
#include "raylib.h"

class AudioManager
{
public:
    // ── Constructor / Destructor ────────────────────
    AudioManager();
    ~AudioManager();

    // ── Music ────────────────────
    void UpdateMusic();
    void PlayMenuMusic();
    void PlayGameMusic();
    void PlayEndMusic();
    void StopMusic();

    // ── Sound Effects ────────────────────
    void PlayHit();
    void PlayMiss();
    void PlaySunk();

private:
    // ── Methods ────────────────────
    void LoadSounds();
    void UnloadSounds() const;

    // ── Member Variables ────────────────────
    Music m_menuMusic  = {};
    Music m_gameMusic  = {};
    Music m_endMusic   = {};
    Music* m_pCurrentMusic = nullptr;

    Sound m_hitSound   = {};
    Sound m_missSound  = {};
};