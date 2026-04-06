#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(int boardSize)
    : Player("Player", boardSize)
{}

void HumanPlayer::SetPendingMove(std::string move)
{
    m_pendingMove = std::move(move);
}

std::string HumanPlayer::MakeMove()
{
    std::string move = std::move(m_pendingMove);
    m_pendingMove.clear();
    return move;
}