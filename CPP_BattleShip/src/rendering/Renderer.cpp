#include <cmath>
#include <vector>
#include "Renderer.h"
#include "../Colors.h"

Renderer::Renderer(const int screenWidth, const int screenHeight) : m_screenWidth(screenWidth), 
    m_screenHeight(screenHeight)
{
}

void Renderer::DrawBackground() const
{
    ClearBackground(GameColors::BACKGROUND);
    DrawRectangle(0, 0, m_screenWidth, m_screenHeight / 8, GameColors::PANEL);
}

void Renderer::DrawCell(const int x, const int y, const char gameCell, const char hiddenCell, const bool isHovered,
    const bool isLastShot) const
{
    Color cellColor = GameColors::CELL_EMPTY;

    // Base color: ship or empty
    if (gameCell == 'S' || gameCell == 'H')
        cellColor = GameColors::CELL_SHIP;

    // Overlays that change the background color
    if (isLastShot)
        cellColor = GameColors::TEXT_WARNING;
    else if (isHovered)
        cellColor = GameColors::CELL_HOVER;

    DrawRectangle(x, y, m_cellSize - 1, m_cellSize - 1, cellColor);

    // Draw X or O symbol on top — does not change background
    if (hiddenCell == 'X')
    {
        constexpr int fontSize = 30;
        const int textX = x + (m_cellSize - MeasureText("X", fontSize)) / 2;
        const int textY = y + (m_cellSize - fontSize) / 2;
        DrawText("X", textX, textY, fontSize, GameColors::CELL_HIT);
    }
    else if (hiddenCell == 'O')
    {
        constexpr int fontSize = 25;
        const int textX = x + (m_cellSize - MeasureText("O", fontSize)) / 2;
        const int textY = y + (m_cellSize - fontSize) / 2;
        DrawText("O", textX, textY, fontSize, GameColors::CELL_MISS);
    }
}

void Renderer::TriggerShake(const float intensity, const float duration)
{
    m_shakeIntensity = intensity;
    m_shakeDuration  = duration;
    m_shakeTimer     = duration;
}

void Renderer::UpdateShake(const float deltaTime)
{
    if (m_shakeTimer <= 0.0f)
    {
        m_shakeOffsetX = 0;
        m_shakeOffsetY = 0;
        return;
    }

    m_shakeTimer -= deltaTime;

    const float strength = (m_shakeTimer / m_shakeDuration) * m_shakeIntensity;
    m_shakeOffsetX = (GetRandomValue(0, 1) == 0 ? 1 : -1) * static_cast<int>(strength);
    m_shakeOffsetY = (GetRandomValue(0, 1) == 0 ? 1 : -1) * static_cast<int>(strength);
}

void Renderer::DrawLabels(const int originX, const int originY, const int boardSize) const
{
    for (int i = 0; i < boardSize; i++)
    {
        // Column numbers above the board (1, 2, 3, ...)
        const std::string colLabel = std::to_string(i + 1);
        DrawText(colLabel.c_str(),
                 originX + i * m_cellSize + m_cellSize / 4,
                 originY - 20,
                 16,
                 GameColors::TEXT_DIM);

        // Row letters left of the board (A, B, C, ...)
        const char rowLabel = static_cast<char>('A' + i);
        DrawText(TextFormat("%c", rowLabel),
                 originX - 20,
                 originY + i * m_cellSize + m_cellSize / 4,
                 16,
                 GameColors::TEXT_DIM);
    }
}

void Renderer::DrawBoard(const Board& board, const int originX, const int originY, const bool hideShips, const bool interactive,
    const int lastShotRow, const int lastShotCol) const
{
    const int boardSize = board.GetBoardSize();
    const auto& gameBoard = board.GetGameBoard();
    const auto& hiddenBoard = board.GetHiddenBoard();

    const int mouseX = GetMouseX();
    const int mouseY = GetMouseY();

    for (int row = 0; row < boardSize; row++)
    {
        for (int col = 0; col < boardSize; col++)
        {
            const int x = originX + col * m_cellSize + m_shakeOffsetX;
            const int y = originY + row * m_cellSize + m_shakeOffsetY;

            const bool isHovered = interactive &&
                mouseX >= x && mouseX < x + m_cellSize &&
                mouseY >= y && mouseY < y + m_cellSize;

            const bool isLastShot = (row == lastShotRow && col == lastShotCol);

            // For AI board: hide ships unless hit
            const char gameCell = hideShips ? '-' : gameBoard[row][col];

            DrawCell(x, y, gameCell, hiddenBoard[row][col], isHovered, isLastShot);
        }
    }
}

void Renderer::DrawPlayerBoard(const Board& board, const int originX, const int originY,
    const int lastShotRow, const int lastShotCol) const
{
    const int boardSize = board.GetBoardSize();
    const auto& gameBoard = board.GetGameBoard();
    const auto& hiddenBoard = board.GetHiddenBoard();

    for (int row = 0; row < boardSize; row++)
    {
        for (int col = 0; col < boardSize; col++)
        {
            const int x = originX + col * m_cellSize + m_shakeOffsetX;
            const int y = originY + row * m_cellSize + m_shakeOffsetY;

            const bool isLastShot = (row == lastShotRow && col == lastShotCol);

            // Show ships from gameBoard, X/O from hiddenBoard
            DrawCell(x, y, gameBoard[row][col], hiddenBoard[row][col], false, isLastShot);
        }
    }
}

void Renderer::DrawBoardTitle(const std::string& title, const int originX, const int originY, const int boardSize) const
{
    constexpr int fontSize = 18;
    const int textWidth = MeasureText(title.c_str(), fontSize);
    const int boardWidth = boardSize * m_cellSize;
    DrawText(title.c_str(), originX + (boardWidth - textWidth) / 2, originY - 45, fontSize, GameColors::TEXT_ACCENT);
}

void Renderer::DrawShipCount(const Board& board, const int originX, const int originY) const
{
    constexpr int fontSize = 16;
    const int boardHeight = board.GetBoardSize() * m_cellSize;
    const std::string text = "Ships remaining: " + std::to_string(board.GetRemainingShips());
    DrawText(text.c_str(), originX, originY + boardHeight + 10, fontSize, GameColors::TEXT_NORMAL);
}

void Renderer::DrawShipOutlines(const Board& board, int originX, int originY, bool sunkOnly) const
{
    constexpr auto outlineColor = GameColors::CELL_OUTLINE;

    for (const Ship& ship : board.GetShips())
    {
        if (sunkOnly && !ship.IsSunk())
            continue;
        
        constexpr int thickness = 2;
        const int startX = originX + ship.m_startCol * m_cellSize;
        const int startY = originY + ship.m_startRow * m_cellSize;

        const int width  = ship.m_bHorizontal ? ship.m_length * m_cellSize - 1 : m_cellSize - 1;
        const int height = ship.m_bHorizontal ? m_cellSize - 1 : ship.m_length * m_cellSize - 1;

        DrawRectangleLinesEx({ static_cast<float>(startX), static_cast<float>(startY),
              static_cast<float>(width),  static_cast<float>(height) }, thickness, outlineColor);
    }
}

void Renderer::DrawSeparator() const
{
    const int x = m_screenWidth / 2;
    const int topY = m_screenHeight / 8;
    const int bottomY = m_screenHeight - 40;
    DrawRectangle(x - 1, topY, 2, bottomY - topY, GameColors::GRID_LINE);
}

bool Renderer::ScreenToBoard(const int mouseX, const int mouseY, const int originX, const int originY,
    const int boardSize, int& outRow, int& outCol) const
{
    const int relX = mouseX - originX;
    const int relY = mouseY - originY;

    if (relX < 0 || relY < 0)
        return false;

    outCol = relX / m_cellSize;
    outRow = relY / m_cellSize;

    if (outRow >= boardSize || outCol >= boardSize)
        return false;

    return true;
}

void Renderer::DrawStatusText(const std::string& text, const Color color) const
{
    constexpr int fontSize = 20;
    const int textWidth = MeasureText(text.c_str(), fontSize);
    const int x = (m_screenWidth - textWidth) / 2;
    const int y = m_screenHeight / 16;

    DrawText(text.c_str(), x, y, fontSize, color);
}

void Renderer::DrawStats(const int playerHits, const int playerShots, const int aiHits, const int aiShots) const
{
    constexpr int fontSize = 16;
    constexpr int padding = 10;

    const float playerHitRate = playerShots > 0
        ? static_cast<float>(playerHits) / static_cast<float>(playerShots) * 100.0f : 0.0f;

    const float aiHitRate = aiShots > 0
        ? static_cast<float>(aiHits) / static_cast<float>(aiShots) * 100.0f : 0.0f;

    // Player stats — left side
    DrawText(TextFormat("Player  Hits: %d | Shots: %d | Hitrate: %.1f%%", playerHits, playerShots, playerHitRate),
             padding, m_screenHeight - 30, fontSize, GameColors::TEXT_SUCCESS);

    // AI stats — right side
    const std::string aiText = TextFormat("AI  Hits: %d | Shots: %d | Hitrate: %.1f%%", aiHits, aiShots, aiHitRate);
    const int aiTextWidth = MeasureText(aiText.c_str(), fontSize);
    DrawText(aiText.c_str(), m_screenWidth - aiTextWidth - padding, m_screenHeight - 30, fontSize, 
        GameColors::TEXT_ERROR);
}

void Renderer::DrawMenuScreen(int boardSize, float timer, const std::vector<int>& shipLengths) const
{
    // ── Animated Waves ────────────────────
    for (int x = 0; x < m_screenWidth; x += 4)
    {
        const float wave1 = std::sin(x * 0.02f + timer * 1.5f) * 15.0f;
        const float wave2 = std::sin(x * 0.035f + timer * 2.0f) * 10.0f;

        const int y1 = static_cast<int>(m_screenHeight * 0.42f + wave1);
        const int y2 = static_cast<int>(m_screenHeight * 0.45f + wave2);

        DrawRectangle(x, y1, 4, 3, GameColors::WAVE_MENU_1);
        DrawRectangle(x, y2, 4, 3, GameColors::WAVE_MENU_2);
    }

    // ── Title ────────────────────
    const float pulse = (std::sin(timer * 2.0f) + 1.0f) / 2.0f;
    const unsigned char titleAlpha = static_cast<unsigned char>(180 + pulse * 75.0f);

    constexpr int titleFontSize = 50;
    const char* title = "BATTLESHIP - RAYLIB EDITION";
    const int titleWidth = MeasureText(title, titleFontSize);
    DrawText(title, (m_screenWidth - titleWidth) / 2, static_cast<int>(m_screenHeight * 0.12f), titleFontSize,
             { GameColors::TEXT_ACCENT.r,
               GameColors::TEXT_ACCENT.g,
               GameColors::TEXT_ACCENT.b,
               titleAlpha });

    // ── Subtitle ────────────────────
    constexpr int subtitleFontSize = 18;
    const auto subtitle = "Sink all enemy ships before the AI sinks yours!";
    const int subtitleWidth = MeasureText(subtitle, subtitleFontSize);
    DrawText(subtitle, (m_screenWidth - subtitleWidth) / 2, static_cast<int>(m_screenHeight * 0.22f),
        subtitleFontSize, GameColors::TEXT_DIM);
    
    // ── Board Size Selector ────────────────────
    const int leftBlockX = m_screenWidth / 4;
    const int blockY = static_cast<int>(m_screenHeight * 0.52f);

    constexpr int labelFontSize = 18;
    constexpr int sizeFontSize  = 42;

    const auto sizeLabel = "BOARD SIZE";
    const int sizeLabelWidth = MeasureText(sizeLabel, labelFontSize);
    DrawText(sizeLabel, leftBlockX - sizeLabelWidth / 2, blockY - 30, labelFontSize, GameColors::TEXT_DIM);

    // ── < Arrow ────────────────────
    DrawText("<", leftBlockX - 80, blockY + 5, sizeFontSize, GameColors::TEXT_WARNING);

    // ── Board size number ────────────────────
    const std::string sizeStr = std::to_string(boardSize) + "x" + std::to_string(boardSize);
    const int sizeStrWidth = MeasureText(sizeStr.c_str(), sizeFontSize);
    DrawText(sizeStr.c_str(), leftBlockX - sizeStrWidth / 2, blockY, sizeFontSize, GameColors::TEXT_NORMAL);

    // ── > Arrow ────────────────────
    DrawText(">", leftBlockX + 60, blockY + 5, sizeFontSize, GameColors::TEXT_WARNING);

    // ── Preview Grid ────────────────────
    const int previewX = static_cast<int>(m_screenWidth * 0.62f);
    const int previewY = static_cast<int>(m_screenHeight * 0.30f);

    constexpr int maxPreviewSize = 220;
    const int cellSize = maxPreviewSize / boardSize;
    const int actualSize = cellSize * boardSize;

    const auto previewLabel = "PREVIEW";
    const int previewLabelWidth = MeasureText(previewLabel, labelFontSize);
    DrawText(previewLabel, previewX + actualSize / 2 - previewLabelWidth / 2, previewY - 30, labelFontSize,
        GameColors::TEXT_DIM);

    for (int row = 0; row < boardSize; row++)
    {
        for (int col = 0; col < boardSize; col++)
        {
            // Alternating cell colors for better visibility
            const Color cellColor = (row + col) % 2 == 0 ? Color{ 35, 45, 90, 255 }
                : Color{ 25, 32, 68, 255 };

            DrawRectangle(previewX + col * cellSize, previewY + row * cellSize, cellSize - 1, cellSize - 1,
                cellColor);
        }
    }
    
    // ── Fleet Overview ────────────────────
    const int fleetX = leftBlockX - 100;
    const int fleetY = blockY + 80;
    constexpr int fleetFontSize = 16;
    constexpr int fleetLineHeight = 28;

    const auto fleetLabel = "YOUR FLEET:";
    DrawText(fleetLabel, fleetX, fleetY, fleetFontSize, GameColors::TEXT_DIM);

    // Build ship lengths for current board size
    std::vector<std::pair<int, int>> shipGroups;
    const std::vector<int>& lengths = shipLengths;

    // Group by length: count how many of each
    for (const int len : lengths)
    {
        bool found = false;
        for (auto& pair : shipGroups)
        {
            if (pair.first == len) { pair.second++; found = true; break; }
        }
        if (!found) shipGroups.emplace_back(len, 1);
    }

    // Draw each group
    for (int i = 0; i < static_cast<int>(shipGroups.size()); i++)
    {
        const int count  = shipGroups[i].second;
        const int length = shipGroups[i].first;
        const int lineY  = fleetY + fleetLineHeight * (i + 1);

        // Count label
        DrawText(TextFormat("%dx", count), fleetX, lineY, fleetFontSize, GameColors::TEXT_NORMAL);

        // Ship block visualization
        constexpr int blockW = 18;
        constexpr int blockH = 14;
        constexpr int blockSpacing = 2;
        const int blocksStartX = fleetX + 40;

        for (int b = 0; b < length; b++)
        {
            DrawRectangle(blocksStartX + b * (blockW + blockSpacing), lineY, blockW, blockH, 
                GameColors::CELL_SHIP);
        }

        // ── L-label ────────────────────
        DrawText(TextFormat("L%d", length), blocksStartX + length * (blockW + blockSpacing) + 8,lineY, 
            fleetFontSize, GameColors::TEXT_DIM);
    }

    // ── Enter Button ────────────────────
    const float enterPulse = (std::sin(timer * 5.0f) + 1.0f) / 2.0f;
    const unsigned char enterAlpha = static_cast<unsigned char>(150 + enterPulse * 105.0f);

    constexpr int enterFontSize = 25;
    const auto enterText = "[ PRESS ENTER TO START ]";
    const int enterWidth = MeasureText(enterText, enterFontSize);
    DrawText(enterText, (m_screenWidth - enterWidth) / 2, static_cast<int>(m_screenHeight * 0.88f),
        enterFontSize,
        { GameColors::TEXT_SUCCESS.r,
          GameColors::TEXT_SUCCESS.g,
          GameColors::TEXT_SUCCESS.b,
          enterAlpha });
}

void Renderer::DrawGameOverScreen(const bool playerWon, const int playerHits, const int playerShots, const int aiHits,
    const int aiShots, const float timer) const
{
    // ── Background Effect ────────────────────
    for (int x = 0; x < m_screenWidth; x += 4)
    {
        const float wave = std::sin(x * 0.02f + timer * 2.0f) * 12.0f;
        const int y = static_cast<int>(m_screenHeight * 0.55f + wave);

        const Color waveColor = playerWon ? GameColors::WAVE_WIN : GameColors::WAVE_LOSE;
        DrawRectangle(x, y, 4, 3, waveColor);
    }

    // ── Result Title ────────────────────
    const float pulse = (std::sin(timer * 2.5f) + 1.0f) / 2.0f;
    const unsigned char titleAlpha = static_cast<unsigned char>(180 + pulse * 75.0f);

    constexpr int titleFontSize = 60;
    const char* resultText = playerWon ? "YOU WIN!" : "YOU LOSE!";
    const Color baseColor = playerWon ? GameColors::TEXT_SUCCESS : GameColors::TEXT_ERROR;
    const Color titleColor = { baseColor.r, baseColor.g, baseColor.b, titleAlpha };

    const int titleWidth = MeasureText(resultText, titleFontSize);
    DrawText(resultText, (m_screenWidth - titleWidth) / 2, static_cast<int>(m_screenHeight * 0.12f),
        titleFontSize, titleColor);

    // ── Subtitle ────────────────────
    constexpr int subtitleFontSize = 18;
    const char* subtitle = playerWon ? "Congratulations! You sank all enemy ships!"
        : "The AI sank all your ships. Better luck next time!";
    const int subtitleWidth = MeasureText(subtitle, subtitleFontSize);
    DrawText(subtitle, (m_screenWidth - subtitleWidth) / 2, static_cast<int>(m_screenHeight * 0.26f),
        subtitleFontSize, GameColors::TEXT_DIM);

    // ── Stats Panels ────────────────────
    const float playerHitRate = playerShots > 0
        ? static_cast<float>(playerHits) / static_cast<float>(playerShots) * 100.0f : 0.0f;

    const float aiHitRate = aiShots > 0
        ? static_cast<float>(aiHits) / static_cast<float>(aiShots) * 100.0f : 0.0f;

    constexpr int panelW = 280;
    constexpr int panelH = 160;
    constexpr int panelY = 300;
    const int playerPanelX = m_screenWidth / 2 - panelW - 40;
    const int aiPanelX     = m_screenWidth / 2 + 40;

    // ── Player panel background ────────────────────
    DrawRectangle(playerPanelX, panelY, panelW, panelH, GameColors::PANEL);
    DrawRectangleLinesEx({ static_cast<float>(playerPanelX), static_cast<float>(panelY), 
        static_cast<float>(panelW), static_cast<float>(panelH) }, 2, GameColors::TEXT_SUCCESS);

    // ── AI panel background ────────────────────
    DrawRectangle(aiPanelX, panelY, panelW, panelH, GameColors::PANEL);
    DrawRectangleLinesEx({ static_cast<float>(aiPanelX), static_cast<float>(panelY), 
        static_cast<float>(panelW), static_cast<float>(panelH) }, 2, GameColors::TEXT_ERROR);

    constexpr int statsFontSize  = 20;
    constexpr int labelFontSize  = 16;
    constexpr int lineHeight     = 35;
    constexpr int paddingX       = 20;
    constexpr int paddingY       = 20;

    // ── Player stats ────────────────────
    DrawText("Player Stats:", playerPanelX + paddingX, panelY + paddingY, statsFontSize, GameColors::TEXT_SUCCESS);
    DrawText(TextFormat("Hits:     %d", playerHits), playerPanelX + paddingX, panelY + paddingY + lineHeight,
        labelFontSize, GameColors::TEXT_NORMAL);
    DrawText(TextFormat("Shots:    %d", playerShots), playerPanelX + paddingX, panelY + paddingY + lineHeight * 2,
        labelFontSize, GameColors::TEXT_NORMAL);
    DrawText(TextFormat("Hitrate:  %.1f%%", playerHitRate), playerPanelX + paddingX, 
        panelY + paddingY + lineHeight * 3, labelFontSize, GameColors::TEXT_WARNING);

    // ── AI stats ────────────────────
    DrawText("AI Stats:", aiPanelX + paddingX, panelY + paddingY, statsFontSize, GameColors::TEXT_ERROR);
    DrawText(TextFormat("Hits:     %d", aiHits), aiPanelX + paddingX, panelY + paddingY + lineHeight,
        labelFontSize, GameColors::TEXT_NORMAL);
    DrawText(TextFormat("Shots:    %d", aiShots), aiPanelX + paddingX, panelY + paddingY + lineHeight * 2,
        labelFontSize, GameColors::TEXT_NORMAL);
    DrawText(TextFormat("Hitrate:  %.1f%%", aiHitRate), aiPanelX + paddingX, panelY + paddingY + lineHeight * 3,
        labelFontSize, GameColors::TEXT_WARNING);

    // ── Prompt ────────────────────
    const float enterPulse = (std::sin(timer * 3.0f) + 1.0f) / 2.0f;
    const unsigned char enterAlpha = static_cast<unsigned char>(150 + enterPulse * 105.0f);

    constexpr int promptFontSize = 20;
    const auto prompt = "Press ENTER to play again          ESC to quit";
    const int promptWidth = MeasureText(prompt, promptFontSize);
    DrawText(prompt, (m_screenWidth - promptWidth) / 2, static_cast<int>(m_screenHeight * 0.88f),
        promptFontSize,
        { GameColors::TEXT_DIM.r,
            GameColors::TEXT_DIM.g,
            GameColors::TEXT_DIM.b,
            enterAlpha });
}