/*****************************************************************************
 + Project: Battleship - Raylib Edition                                      +
 + File: AIPlayer.h                                                          +
 +                                                                           +
 + AI player with Hunt & Target algorithm.                                   +
 + Hunt mode: shoots randomly until a hit.                                   +
 + Target mode: after a hit, systematically attacks adjacent cells           +
 + to sink the ship.                                                         +
 *****************************************************************************/

#pragma once

#include <vector>
#include <random>
#include "Player.h"
#include "Types.h"

class AIPlayer : public Player
{
public:
    // ── Constructor / Destructor ────────────────────
    explicit AIPlayer(int boardSize);
    ~AIPlayer() override = default;

    // ── Methods ────────────────────
    Coordinate MakeMove() override;
    void EnqueueAdjacentCells(const Coordinate& coord);
    void ClearTargetCells();

private:
    // ── Methods ────────────────────
    Coordinate GetHuntModeMove() const;

    // ── Member Variables ────────────────────
    // Target mode: queue of cells to shoot at after a hit
    std::vector<Coordinate> m_targetCells = {};
    mutable std::mt19937 m_rng = std::mt19937(std::random_device{}()); //for hunt mode
};