/*****************************************************************************
+ Project: Battleship - Raylib Edition                                      +
 + File: Player.h                                                            +
 +                                                                           +
 + Abstract base class for all player types.                                 +
 + Defines the interface Game uses to interact with any player,              +
 + whether human or AI (Polymorphism).                                       +
 + RAII: Board memory is managed via unique_ptr.                             +
 *****************************************************************************/

#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Board.h"

class Player
{
public:
    // ── Constructor / Destructor ──────────────────────────────────────────
    Player(std::string name, int boardSize);
    virtual ~Player() = default;

    // ── Methods ───────────────────────────────────────────────────────────
    // Pure virtual: every subclass must implement its own move logic.
    // This is the core of polymorphism — Game calls MakeMove() without
    // knowing whether it's talking to a human or an AI.
    virtual std::string MakeMove() = 0;

    void UpdateMoveHistory(int row, int column);
    [[nodiscard]] bool IsValidMove(int row, int column) const;

    // ── Getters ───────────────────────────────────────────────────────────
    [[nodiscard]] const std::string& GetName() const { return m_name; }
    [[nodiscard]] Board* GetBoard() const { return m_pBoard.get(); }

protected:
    // ── Member Variables ──────────────────────────────────────────────────
    std::string m_name;
    int m_boardSize = 0;

    // unique_ptr: Player owns the Board.
    // No manual delete needed, no memory leak possible.
    std::unique_ptr<Board> m_pBoard = nullptr;

    std::vector<std::vector<bool>> m_moveHistory = {};
};