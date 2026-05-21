#pragma once
#include "TimeManager.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "IGame.h"
#include <vector>
#include <string>

struct GameEntry
{
    char        key = ' ';
    std::string name;
    IGame* game = nullptr;
};

class GameEngine
{
private:
    bool exitEngine = false;

    TimeManager* timeManager = nullptr;
    DrawManager* drawManager = nullptr;
    InputManager* inputManager = nullptr;

    std::vector<GameEntry> games;
    IGame* currentGame = nullptr;

    int screenHeight = 0;
    int screenWidth = 0;

    void GameLoop();
    void Update();
    void Draw();

    void DrawMenu();
    int  ShowMenuAndWaitForSelection();

public:
    GameEngine(int framesPerSecond, int screenHeight, int screenWidth);
    ~GameEngine();

    void RegisterGame(char key, const std::string& name, IGame* game);

    void Start();
};