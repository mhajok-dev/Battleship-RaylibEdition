#include "Player.h"

Player::Player(std::string name, int boardSize) : m_name(std::move(name)), m_boardSize(boardSize)
{
    m_pBoard = std::make_unique<Board>(boardSize);
    m_moveHistory = std::vector<std::vector<bool>>(boardSize, std::vector<bool>(boardSize, false));
}

void Player::UpdateMoveHistory(const int row, const int column)
{
    if (row >= 0 && row < m_boardSize && column >= 0 && column < m_boardSize)
        m_moveHistory[row][column] = true;
}

bool Player::IsValidMove(const int row, const int column) const
{
    return row >= 0 && row < m_boardSize && column >= 0 && column < m_boardSize && !m_moveHistory[row][column];
}