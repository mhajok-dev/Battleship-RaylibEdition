/*****************************************************************************
 + Project: Battleship - Raylib Edition                                      +
 + File: Ship.h                                                              +
 +                                                                           +
 + Represents a single ship on the board.                                    +
 + A ship occupies consecutive cells either horizontally or vertically.      +
 *****************************************************************************/

#pragma once

struct Ship
{
    // ── Member Variables ────────────────────
    int m_startRow   = 0;
    int m_startCol   = 0;
    int m_length     = 0;
    int m_hits       = 0;
    bool m_bHorizontal = true;

    // ── Methods ────────────────────
    [[nodiscard]] bool IsSunk() const { return m_hits >= m_length; }

    // Returns true if this ship occupies the given cell
    [[nodiscard]] bool OccupiesCell(const int row, const int col) const
    {
        if (m_bHorizontal)
            return row == m_startRow && col >= m_startCol && col < m_startCol + m_length;

        return col == m_startCol && row >= m_startRow && row < m_startRow + m_length;
    }
};