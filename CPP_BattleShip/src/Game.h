/*****************************************************************************
 + Project: Battleship - Raylib Edition                                      +
 + File: Game.h                                                              +
 +                                                                           +
 + Manages game state, player turns and win conditions.                      +
 + SRP: Coordinates between players, board and renderer.                     +
 + Uses a state machine to control the game flow.                            +
 *****************************************************************************/

#pragma once

#include <memory>
#include <random>
#include <vector>
#include "core/Board.h"
#include "core/HumanPlayer.h"
#include "core/AIPlayer.h"
#include "core/Types.h"
#include "rendering/Renderer.h"
#include "audio/AudioManager.h"

enum class GameState : uint8_t
{
    MENU,
    PLAYER_TURN,
    AI_TURN,
    GAME_OVER
};

class Game
{
public:
    // ── Constructor / Destructor ────────────────────
    Game(int screenWidth, int screenHeight);
    ~Game() = default;

    // ── Public methods ────────────────────
    void Update(float deltaTime);
    void Draw() const;

private:
    // ── Methods ────────────────────
    void StartNewGame();
    void PlaceShipsRandomly(Board& board) const;
    void ProcessMenuInput();
    void ProcessPlayerInput();
    void ProcessAITurn(float deltaTime);
    void SwitchToPlayerTurn();
    void SwitchToAITurn();
    void CheckWinCondition();
    
    [[nodiscard]] std::vector<int> BuildFleetConfiguration() const;
    [[nodiscard]] bool CanPlaceShip(const Board& board, const Ship& ship) const;
    [[nodiscard]] ShotResult ApplyShot(Board& board, int row, int col);

    // ── Member Variables ────────────────────
    Renderer m_renderer;
    AudioManager m_audioManager;
    
    std::unique_ptr<HumanPlayer> m_pHumanPlayer = nullptr;
    std::unique_ptr<AIPlayer> m_pAIPlayer = nullptr;
    
    mutable std::mt19937 m_rng = std::mt19937(std::random_device{}()); // for random ships placing
    
    std::string m_lastStatusMessage;
    Coordinate m_lastAIShot = { -1, -1 };

    GameState m_state = GameState::MENU;

    int m_boardSize = 5;
    int m_aiOriginX = 0;
    int m_aiOriginY = 0;
    int m_playerOriginX = 0;
    int m_playerOriginY = 0;

    float m_aiTimer = .0f;
    float m_aiDelay = .0f;
    float m_switchTimer = .0f;
    float m_menuTimer = .0f;
    
    bool m_bWaitingToSwitch = false;
    bool m_bPlayerWon = false;
};