#include "Board.h"

Board::Board(int boardSize)
{
    InitializeBoard(boardSize);
}

bool Board::WasAlreadyShot(int row, int col) const
{
    return m_hiddenBoard[row][col] == 'X' || m_hiddenBoard[row][col] == 'O';
}

void Board::InitializeBoard(int size)
{
    m_boardSize = size;
    m_shotsFired = 0;
    m_hits = 0;

    m_gameBoard = std::vector(size, std::vector(size, '-'));
    m_hiddenBoard = std::vector(size, std::vector(size, '-'));
}

void Board::AddShip(const Ship& ship)
{
    m_ships.push_back(ship);
    m_shipsRemaining++;

    // Mark ship cells on the game board
    for (int i = 0; i < ship.m_length; i++)
    {
        const int row = ship.m_bHorizontal ? ship.m_startRow : ship.m_startRow + i;
        const int col = ship.m_bHorizontal ? ship.m_startCol + i : ship.m_startCol;
        m_gameBoard[row][col] = 'S';
    }
}

Ship* Board::FindShipAtCell(const int row, const int column)
{
    for (Ship& ship : m_ships)
    {
        if (ship.OccupiesCell(row, column))
            return &ship;
    }
    return nullptr;
}

bool Board::AllShipsSunk() const
{
    for (const Ship& ship : m_ships)
    {
        if (!ship.IsSunk())
            return false;
    }
    return true;
}