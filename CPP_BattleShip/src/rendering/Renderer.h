/*****************************************************************************
 + Project: Battleship - Raylib Edition                                      +
 + File: Renderer.h                                                          +
 +                                                                           +
 + Handles all drawing via raylib.                                           +
 + SRP: Only responsible for rendering, no game logic or input handling.     +
 *****************************************************************************/

#pragma once
#include <string>
#include "raylib.h"
#include "../core/Board.h"

class Renderer
{
public:
    // ── Constructor / Destructor ──────────────────────────────────────────
    Renderer(int screenWidth, int screenHeight);
    ~Renderer() = default;

    // ── Methods ───────────────────────────────────────────────────────────
    void DrawBackground() const;
    void DrawBoardTitle(const std::string& title, int originX, int originY, int boardSize) const;
    void DrawBoard(const Board& board, int originX, int originY, bool hideShips, bool interactive,
        int lastShotRow = -1, int lastShotCol = -1) const;
    void DrawPlayerBoard(const Board& board, int originX, int originY,
                     int lastShotRow = -1, int lastShotCol = -1) const;
    void DrawShipCount(const Board& board, int originX, int originY) const;
    void DrawShipOutlines(const Board& board, int originX, int originY, bool sunkOnly = false) const;
    void DrawSeparator() const;
    void DrawLabels(int originX, int originY, int boardSize) const;
    void DrawStatusText(const std::string& text, Color color) const;
    void DrawStats(int playerHits, int playerShots, int aiHits, int aiShots) const;
    void DrawMenuScreen(int boardSize, float timer) const;
    void DrawGameOverScreen(bool playerWon, int playerHits, int playerShots, int aiHits, int aiShots, float timer) const;
    
    void TriggerShake(float intensity = 8.0f, float duration = 0.3f);
    void UpdateShake(float deltaTime);

    // ── Getters ───────────────────────────────────────────────────────────
    [[nodiscard]] int GetCellSize()   const { return m_cellSize;   }
    [[nodiscard]] int GetScreenWidth()  const { return m_screenWidth;  }
    [[nodiscard]] int GetScreenHeight() const { return m_screenHeight; }
    [[nodiscard]] int GetShakeOffsetX() const { return m_shakeOffsetX; }
    [[nodiscard]] int GetShakeOffsetY() const { return m_shakeOffsetY; }

    // Converts a screen position to board coordinates.
    // Returns false if the position is outside the board.
    [[nodiscard]] bool ScreenToBoard(int mouseX, int mouseY, int originX, int originY, int boardSize, 
                                    int& outRow, int& outCol) const;

private:
    // ── Methods ───────────────────────────────────────────────────────────
    void DrawCell(int x, int y, char gameCell, char hiddenCell, bool isHovered, bool isLastShot) const;

    // ── Member Variables ──────────────────────────────────────────────────
    int m_screenWidth  = 0;
    int m_screenHeight = 0;
    int m_cellSize     = 40;
    
    float m_shakeTimer     = 0.0f;
    float m_shakeDuration  = 0.3f;
    float m_shakeIntensity = 0.0f;
    
    int   m_shakeOffsetX   = 0;
    int   m_shakeOffsetY   = 0;
};