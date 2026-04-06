#include "AIPlayer.h"

AIPlayer::AIPlayer(int boardSize) : Player("AI", boardSize)
{
}

Coordinate AIPlayer::GetHuntModeMove() const
{
    std::uniform_int_distribution<int> dist(0, m_boardSize - 1);
    Coordinate coord;

    do
    {
        coord.row = dist(m_rng);
        coord.column = dist(m_rng);
    }
    while (!IsValidMove(coord.row, coord.column));

    return coord;
}

void AIPlayer::EnqueueAdjacentCells(const Coordinate& coord)
{
    const std::vector<Coordinate> adjacent =
    {
        { coord.row - 1, coord.column },
        { coord.row + 1, coord.column },
        { coord.row, coord.column - 1 },
        { coord.row, coord.column + 1 }
    };

    for (const Coordinate& cell : adjacent)
    {
        if (IsValidMove(cell.row, cell.column))
            m_targetCells.push_back(cell);
    }
}

void AIPlayer::ClearTargetCells()
{
    m_targetCells.clear();
}

Coordinate AIPlayer::MakeMove()
{
    Coordinate coord;

    if (!m_targetCells.empty())
    {
        coord = m_targetCells.front();
        m_targetCells.erase(m_targetCells.begin());
    }
    else
        coord = GetHuntModeMove();

    UpdateMoveHistory(coord.row, coord.column);
    return coord;
}