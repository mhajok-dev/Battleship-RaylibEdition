/*****************************************************************************
+ Project: Battleship - Raylib Edition                                      +
 + File: InputValidator.h                                                    +
 +                                                                           +
 + Validates coordinate strings and board size input.                        +
 + SRP: Only responsible for validation logic, no rendering or game state.   +
 *****************************************************************************/

#pragma once
#include <string>
#include <vector>

struct MoveValidation
{
    bool m_bIsValid = false;
    int m_row = 0;
    int m_column = 0;
    std::string m_message;
};

struct SizeValidation
{
    bool m_bIsValid = false;
    int m_size = 0;
    std::string m_message;
};

class InputValidator
{
public:
    // ── Methods ───────────────────────────────────────────────────────────
    [[nodiscard]] static MoveValidation ValidateMove(const std::string& input, int boardSize,
        const std::vector<std::vector<char>>& hiddenBoard);

    [[nodiscard]] static SizeValidation ValidateBoardSize(const std::string& input, int minSize, int maxSize);
};