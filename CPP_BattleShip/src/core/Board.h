/*****************************************************************************
 + Project: Battleship - Raylib Edition                                      +
 + File: Board.h                                                             +
 +                                                                           +
 + Manages board data, ship positions and shot tracking.                     +
 + SRP: This class only knows its own data. Drawing is handled               +
 + by the Renderer, input validation by the InputValidator.                  +
 *****************************************************************************/

#pragma once
#include <vector>
#include "Ship.h"

class Board
{
public:
    // ── Constructor / Destructor ────────────────────
    explicit Board(int boardSize = 5);
    virtual ~Board() = default;

    // ── Methods ────────────────────
    void AddShip(const Ship& ship);
    Ship* FindShipAtCell(int row, int column);
    [[nodiscard]] bool AllShipsSunk() const;
    void ResetShipCount() { m_shipsRemaining = 0; }

    // ── Getters ────────────────────
    // Non-const references so Game can mutate values directly,
    // e.g. GetRemainingShips()-- after a hit
    std::vector<std::vector<char>>& GetGameBoard()   { return m_gameBoard;   }
    std::vector<std::vector<char>>& GetHiddenBoard() { return m_hiddenBoard; }

    // Const overloads for read-only access (e.g. Renderer)
    [[nodiscard]] const std::vector<std::vector<char>>& GetGameBoard()   const { return m_gameBoard;   }
    [[nodiscard]] const std::vector<std::vector<char>>& GetHiddenBoard() const { return m_hiddenBoard; }

    void IncrementShotsFired()    { m_shotsFired++;     }
    void IncrementHits()          { m_hits++;           }
    void DecrementRemainingShips(){ m_shipsRemaining--; }
    
    [[nodiscard]] int GetShotsFired()     const { return m_shotsFired;     }
    [[nodiscard]] int GetHits()           const { return m_hits;           }
    [[nodiscard]] int GetRemainingShips() const { return m_shipsRemaining; }
    
    [[nodiscard]] int  GetBoardSize()    const { return m_boardSize;           }
    [[nodiscard]] bool GetWinCondition() const { return m_shipsRemaining <= 0; }
    
    [[nodiscard]] std::vector<Ship>& GetShips() { return m_ships; }
    [[nodiscard]] const std::vector<Ship>& GetShips() const { return m_ships; }

private:
    // ── Methods ─────
    void InitializeBoard(int size);

    // ── Member Variables ─────
    int m_boardSize      = 0;
    int m_shipsRemaining = 0;
    int m_shotsFired     = 0;
    int m_hits           = 0;

    // m_gameBoard   → real positions: '-' empty, 'S' ship, 'H' hit ship
    // m_hiddenBoard → opponent view:  '-' unknown, 'X' hit, 'O' miss
    std::vector<std::vector<char>> m_gameBoard   = {};
    std::vector<std::vector<char>> m_hiddenBoard = {};
    std::vector<Ship> m_ships = {};
};