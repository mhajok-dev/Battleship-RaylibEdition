#include "InputValidator.h"
#include <algorithm>
#include <stdexcept>

MoveValidation InputValidator::ValidateMove(const std::string& input, const int boardSize,
    const std::vector<std::vector<char>>& hiddenBoard)
{
    MoveValidation result;

    if (input.length() < 2)
    {
        result.m_message = "Invalid input. Please use format '1A'.";
        return result;
    }

    std::string upper = input;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    // Last character must be a letter (row), everything before must be digits (column)
    if (!std::isalpha(upper.back()))
    {
        result.m_message = "Invalid input. Please use format '1A'.";
        return result;
    }

    for (size_t i = 0; i < upper.length() - 1; i++)
    {
        if (!std::isdigit(upper[i]))
        {
            result.m_message = "Invalid input. Please use format '1A'.";
            return result;
        }
    }

    int col;
    try
    {
        col = std::stoi(upper.substr(0, upper.length() - 1)) - 1;
    }
    catch (const std::invalid_argument&)
    {
        result.m_message = "Invalid input. Please use format '1A'.";
        return result;
    }

    const int row = upper.back() - 'A';

    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize)
    {
        result.m_message = "Coordinates out of bounds. Please try again.";
        return result;
    }

    if (hiddenBoard[row][col] == 'X' || hiddenBoard[row][col] == 'O')
    {
        result.m_message = "You already fired at this position.";
        return result;
    }

    result.m_bIsValid = true;
    result.m_row = row;
    result.m_column = col;
    return result;
}

SizeValidation InputValidator::ValidateBoardSize(const std::string& input, const int minSize, const int maxSize)
{
    SizeValidation result;

    if (input.empty())
    {
        result.m_message = "Please enter a number.";
        return result;
    }

    for (const char c : input)
    {
        if (!std::isdigit(c))
        {
            result.m_message = "Invalid input. Please enter a number without letters.";
            return result;
        }
    }

    try
    {
        const int size = std::stoi(input);

        if (size < minSize || size > maxSize)
        {
            result.m_message = "Please enter a number between " +
                std::to_string(minSize) + " and " + std::to_string(maxSize) + ".";
            return result;
        }

        result.m_bIsValid = true;
        result.m_size = size;
    }
    catch (const std::exception&)
    {
        result.m_message = "Invalid input. Please enter a valid number.";
    }

    return result;
}