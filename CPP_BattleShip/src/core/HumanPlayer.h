/*****************************************************************************
+ Project: Battleship - Raylib Edition                                      +
 + File: HumanPlayer.h                                                       +
 +                                                                           +
 + Human player controlled by mouse input.                                   +
 + Game detects mouse clicks and sets the pending move via SetPendingMove(). +
 + MakeMove() returns that move when called by Game.                         +
 *****************************************************************************/

#pragma once
#include "Player.h"

class HumanPlayer : public Player
{
public:
    // ── Constructor / Destructor ─────
    explicit HumanPlayer(int boardSize);
    ~HumanPlayer() override = default;

    // ── Methods ─────
    std::string MakeMove() override;
    void SetPendingMove(std::string move);

    // ── Getters ─────
    [[nodiscard]] bool HasPendingMove() const { return !m_pendingMove.empty(); }

private:
    // ── Member Variables ─────
    std::string m_pendingMove;
};