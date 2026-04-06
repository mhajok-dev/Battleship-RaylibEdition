#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#include "raylib.h"
#include "src/Game.h"
#include "src/Settings.h"

int main()
{
    InitWindow(GameSettings::WINDOW_WIDTH, GameSettings::WINDOW_HEIGHT, "Battleship - raylib edition");
    SetTargetFPS(GameSettings::TARGET_FPS);

    Game game(GameSettings::WINDOW_WIDTH, GameSettings::WINDOW_HEIGHT);

    while (!WindowShouldClose())
    {
        game.Update(GetFrameTime());
        game.Draw();
    }

    CloseWindow();
    return 0;
}
