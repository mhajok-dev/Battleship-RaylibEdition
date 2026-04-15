#include "Game.h"
#include "Colors.h"
#include "Settings.h"

Game::Game(const int screenWidth, const int screenHeight) : m_renderer(screenWidth, screenHeight), 
    m_aiDelay(GameSettings::AI_DELAY)
{
}

void Game::StartNewGame()
{
    m_pHumanPlayer = std::make_unique<HumanPlayer>(m_boardSize);
    m_pAIPlayer = std::make_unique<AIPlayer>(m_boardSize);

    PlaceShipsRandomly(*m_pHumanPlayer->GetBoard());
    PlaceShipsRandomly(*m_pAIPlayer->GetBoard());

    // Calculate board origins so both boards are centered on screen
    const int totalWidth = m_boardSize * m_renderer.GetCellSize();
    const int screenWidth = m_renderer.GetScreenWidth();
    const int screenHeight = m_renderer.GetScreenHeight();

    m_playerOriginX = screenWidth / 4 - totalWidth / 2;
    m_playerOriginY = screenHeight / 4 + 30;
    m_aiOriginX = screenWidth * 3 / 4 - totalWidth / 2;
    m_aiOriginY = screenHeight / 4 + 30;

    m_bPlayerWon = false;
    m_aiTimer = 0.0f;
    m_state = GameState::PLAYER_TURN;
}

std::vector<int> Game::BuildFleetConfiguration() const
{
    if (m_boardSize <= 6)  return { 3, 2, 2 };
    if (m_boardSize <= 8)  return { 4, 3, 3, 2, 2 };
    if (m_boardSize <= 10) return { 5, 4, 4, 3, 3, 3, 2, 2, 2, 2 };
    if (m_boardSize <= 13) return { 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2 };
    return                 { 6, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2 };
}

bool Game::CanPlaceShip(const Board& board, const Ship& ship) const
{
    for (int i = 0; i < ship.m_length; i++)
    {
        const int row = ship.m_bHorizontal ? ship.m_startRow : ship.m_startRow + i;
        const int col = ship.m_bHorizontal ? ship.m_startCol + i : ship.m_startCol;

        // Out of bounds check
        if (row < 0 || row >= m_boardSize || col < 0 || col >= m_boardSize)
            return false;

        // Collision check
        if (board.GetGameBoard()[row][col] == 'S')
            return false;
    }
    return true;
}

ShotResult Game::ApplyShot(Board& board, const int row, const int col)
{
    board.IncrementShotsFired();

    if (board.GetGameBoard()[row][col] == 'S')
    {
        board.GetHiddenBoard()[row][col] = 'X';
        board.GetGameBoard()[row][col]   = 'H';
        board.IncrementHits();

        Ship* pHitShip = board.FindShipAtCell(row, col);
        if (pHitShip != nullptr)
        {
            pHitShip->m_hits++;
            if (pHitShip->IsSunk())
            {
                board.DecrementRemainingShips();
                // Stronger shake for sunk
                m_renderer.TriggerShake(10.0f, 0.4f);
                return ShotResult::SUNK;
            }
        }
        // Lighter shake for hit
        m_renderer.TriggerShake(6.0f, 0.25f);
        return ShotResult::HIT;
    }

    board.GetHiddenBoard()[row][col] = 'O';
    return ShotResult::MISS;
}

void Game::PlaceShipsRandomly(Board& board) const
{
    const std::vector<int> shipLengths = BuildFleetConfiguration();
    std::uniform_int_distribution<int> posDist(0, m_boardSize - 1);
    std::uniform_int_distribution<int> dirDist(0, 1);

    for (const int length : shipLengths)
    {
        Ship ship;
        ship.m_length = length;
        int attempts = 0;

        do
        {
            ship.m_startRow = posDist(m_rng);
            ship.m_startCol = posDist(m_rng);
            ship.m_bHorizontal = dirDist(m_rng) == 0;
            attempts++;

            if (attempts > 1000)
            {
                // Reset board and start over
                board.GetGameBoard().assign(m_boardSize, std::vector<char>(m_boardSize, '-'));
                board.GetShips().clear();
                board.ResetShipCount();
                attempts = 0;
            }
        }
        while (!CanPlaceShip(board, ship));

        board.AddShip(ship);
    }
}

void Game::ProcessMenuInput()
{
    if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_UP)) && m_boardSize < GameSettings::MAX_BOARD_SIZE)
        m_boardSize++;
    else if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_DOWN)) && m_boardSize > GameSettings::MIN_BOARD_SIZE)
        m_boardSize--;

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        StartNewGame();
}

void Game::ProcessPlayerInput()
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;

    const int mouseX = GetMouseX();
    const int mouseY = GetMouseY();

    int row = 0;
    int col = 0;

    if (!m_renderer.ScreenToBoard(mouseX, mouseY, m_aiOriginX, m_aiOriginY, m_boardSize, row, col))
        return;

    Board* pAIBoard = m_pAIPlayer->GetBoard();
    if (pAIBoard->WasAlreadyShot(row, col))
        return;

    if (const ShotResult result = ApplyShot(*pAIBoard, row, col); result == ShotResult::SUNK)
    {
        m_audioManager.PlaySunk();
        m_lastStatusMessage = "You sunk an enemy ship!";
    }
    else if (result == ShotResult::HIT)
    {
        m_audioManager.PlayHit();
        m_lastStatusMessage = "Hit! Direct hit on an enemy ship!";
    }
    else
    {
        m_audioManager.PlayMiss();
        m_lastStatusMessage = "Miss! No ship there.";
    }

    m_pHumanPlayer->UpdateMoveHistory(row, col);
    CheckWinCondition();

    if (m_state != GameState::GAME_OVER)
        SwitchToAITurn();
}

void Game::SwitchToAITurn()
{
    m_aiTimer = 0.0f;
    m_state = GameState::AI_TURN;
}

void Game::SwitchToPlayerTurn()
{
    m_lastAIShot = { -1, -1 };
    m_lastStatusMessage = "";
    m_state = GameState::PLAYER_TURN;
}

void Game::CheckWinCondition()
{
    if (m_pAIPlayer->GetBoard()->AllShipsSunk())
    {
        m_bPlayerWon = true;
        m_state = GameState::GAME_OVER;
    }
    else if (m_pHumanPlayer->GetBoard()->AllShipsSunk())
    {
        m_bPlayerWon = false;
        m_state = GameState::GAME_OVER;
    }
}

void Game::ProcessAITurn(const float deltaTime)
{
    m_aiTimer += deltaTime;

    if (m_aiTimer < m_aiDelay)
        return;

    Board* pPlayerBoard = m_pHumanPlayer->GetBoard();
    const Coordinate coord = m_pAIPlayer->MakeMove();
    m_lastAIShot = coord;

    if (pPlayerBoard->WasAlreadyShot(coord.row, coord.column))
        return;

    const ShotResult result = ApplyShot(*pPlayerBoard, coord.row, coord.column);
    const std::string coordStr = std::string(1, static_cast<char>('A' + coord.row))
                            + std::to_string(coord.column + 1);

    if (result == ShotResult::SUNK)
    {
        m_pAIPlayer->ClearTargetCells();
        m_audioManager.PlaySunk();
        m_lastStatusMessage = "AI shot at " + coordStr + " - Sunk!";
    }
    else if (result == ShotResult::HIT)
    {
        m_pAIPlayer->EnqueueAdjacentCells({ coord.row, coord.column });
        m_audioManager.PlayHit();
        m_lastStatusMessage = "AI shot at " + coordStr + " - Hit!";
    }
    else
    {
        m_audioManager.PlayMiss();
        m_lastStatusMessage = "AI shot at " + coordStr + " - Miss!";
    }

    CheckWinCondition();

    if (m_state != GameState::GAME_OVER)
    {
        m_bWaitingToSwitch = true;
        m_switchTimer = 0.0f;
    }
}

void Game::Update(const float deltaTime)
{
    m_audioManager.UpdateMusic();
    m_renderer.UpdateShake(deltaTime);
    
    if (m_bWaitingToSwitch)
    {
        m_switchTimer += deltaTime;
        if (m_switchTimer >= GameSettings::SWITCH_DELAY)
        {
            m_bWaitingToSwitch = false;
            SwitchToPlayerTurn();
        }
        return;
    }
    
    switch (m_state)
    {
    case GameState::MENU:
        m_menuTimer += deltaTime;
        m_audioManager.PlayMenuMusic();
        ProcessMenuInput();
        break;
    case GameState::PLAYER_TURN:
        m_audioManager.PlayGameMusic();
        ProcessPlayerInput();
        break;
    case GameState::AI_TURN:
        m_audioManager.PlayGameMusic();
        ProcessAITurn(deltaTime);
        break;
    case GameState::GAME_OVER:
        m_menuTimer += deltaTime;
        m_audioManager.PlayEndMusic();
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            m_state = GameState::MENU;
        else if (IsKeyPressed(KEY_ESCAPE))
            CloseWindow();
        break;
    }
}

void Game::Draw() const
{
    BeginDrawing();
    m_renderer.DrawBackground();

    switch (m_state)
    {
    case GameState::MENU:
        m_renderer.DrawMenuScreen(m_boardSize, m_menuTimer, BuildFleetConfiguration());
        break;

    case GameState::PLAYER_TURN:
    case GameState::AI_TURN:
        m_renderer.DrawSeparator();
        m_renderer.DrawBoardTitle("Your Ships", m_playerOriginX, m_playerOriginY, m_boardSize);
        m_renderer.DrawLabels(m_playerOriginX, m_playerOriginY, m_boardSize);
        m_renderer.DrawPlayerBoard(*m_pHumanPlayer->GetBoard(), m_playerOriginX, m_playerOriginY, m_lastAIShot.row, 
            m_lastAIShot.column);
        m_renderer.DrawShipOutlines(*m_pHumanPlayer->GetBoard(), m_playerOriginX, m_playerOriginY);
        m_renderer.DrawShipCount(*m_pHumanPlayer->GetBoard(), m_playerOriginX, m_playerOriginY);
        m_renderer.DrawBoardTitle("AI's Board", m_aiOriginX, m_aiOriginY, m_boardSize);
        m_renderer.DrawLabels(m_aiOriginX, m_aiOriginY, m_boardSize);
        m_renderer.DrawBoard(*m_pAIPlayer->GetBoard(), m_aiOriginX, m_aiOriginY, true, 
            m_state == GameState::PLAYER_TURN);
        m_renderer.DrawShipOutlines(*m_pAIPlayer->GetBoard(), m_aiOriginX, m_aiOriginY, true);
        m_renderer.DrawShipCount(*m_pAIPlayer->GetBoard(), m_aiOriginX, m_aiOriginY);
        m_renderer.DrawStats(m_pAIPlayer->GetBoard()->GetHits(), m_pAIPlayer->GetBoard()->GetShotsFired(),
            m_pHumanPlayer->GetBoard()->GetHits(), m_pHumanPlayer->GetBoard()->GetShotsFired());
        m_renderer.DrawStatusText(
            m_lastStatusMessage.empty()
                ? (m_state == GameState::PLAYER_TURN
                    ? "Your Turn - click a cell to fire" : "AI is thinking...")
                : m_lastStatusMessage,
            m_lastStatusMessage.find("Hit") != std::string::npos
                ? GameColors::TEXT_ERROR
                : m_lastStatusMessage.find("Miss") != std::string::npos
                    ? GameColors::TEXT_ACCENT : GameColors::TEXT_SUCCESS);
        break;

    case GameState::GAME_OVER:
        m_renderer.DrawGameOverScreen(m_bPlayerWon, m_pAIPlayer->GetBoard()->GetHits(),
            m_pAIPlayer->GetBoard()->GetShotsFired(), m_pHumanPlayer->GetBoard()->GetHits(),
            m_pHumanPlayer->GetBoard()->GetShotsFired(), m_menuTimer);
        break;
    }
    EndDrawing();
}