#include "GameEngine.h"
#include <windows.h>
#include <iostream>
#include <string>

static void HideConsoleCursor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(h, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(h, &info);
}

GameEngine::GameEngine(int framesPerSecond, int screenHeight, int screenWidth)
    : screenHeight(screenHeight)
    , screenWidth(screenWidth)
    , exitEngine(false)
{
    timeManager = new TimeManager(1.0f / (float)framesPerSecond);
    drawManager = new DrawManager(screenHeight, screenWidth);
    inputManager = new InputManager();
}

GameEngine::~GameEngine()
{
    delete timeManager;
    delete drawManager;
    delete inputManager;
}

void GameEngine::RegisterGame(char key, const std::string& name, IGame* game)
{
    GameEntry entry;
    entry.key = key;
    entry.name = name;
    entry.game = game;
    games.push_back(entry);
}

void GameEngine::Start()
{
    HideConsoleCursor();

    while (!exitEngine)
    {
        int selection = ShowMenuAndWaitForSelection();

        if (selection < 0)
        {
            exitEngine = true;
        }
        else
        {
            currentGame = games[selection].game;
            currentGame->Reset();
            timeManager->Reset();

            while (inputManager->IsKeyPressed((int)games[selection].key))
                Sleep(10);

            GameLoop();
        }
    }
}

void GameEngine::DrawMenu()
{
    drawManager->BeginFrame();

    int w = drawManager->GetWidth();
    int h = drawManager->GetHeight();

    for (int i = 1; i < w - 1; ++i)
        drawManager->SetCharInPosition(' ', HUD_ROW_TEXT, i);

    char title[] = "MENU PRINCIPAL";
    int titleLen = 0;
    for (int i = 0; title[i] != '\0'; i++) titleLen++;
    int titleStart = (w / 2) - titleLen / 2;
    if (titleStart < 3) titleStart = 3;
    int col = titleStart;
    for (int i = 0; title[i] != '\0'; i++)
    {
        if (col >= w - 2) break;
        drawManager->SetCharInPosition(title[i], HUD_ROW_TEXT, col++);
    }

    int totalContent = 5 + (int)games.size() * 2;
    int available = (h - 2) - PLAYFIELD_TOP + 1;
    int centerY = PLAYFIELD_TOP + (available - totalContent) / 2;
    if (centerY < PLAYFIELD_TOP + 1) centerY = PLAYFIELD_TOP + 1;

    char subtitle[] = "Selecciona un juego:";
    int subLen = 0;
    for (int i = 0; subtitle[i] != '\0'; i++) subLen++;
    int subStart = (w / 2) - subLen / 2;
    if (subStart < 2) subStart = 2;
    col = subStart;
    for (int i = 0; subtitle[i] != '\0'; i++)
    {
        if (col >= w - 2) break;
        drawManager->SetCharInPosition(subtitle[i], centerY, col++);
    }

    for (size_t g = 0; g < games.size(); g++)
    {
        int row = centerY + 3 + (int)g * 2;
        if (row >= h - 2) break;

        char line[80];
        sprintf_s(line, sizeof(line), "(%c) %s", games[g].key, games[g].name.c_str());

        int lineLen = 0;
        for (int i = 0; line[i] != '\0'; i++) lineLen++;
        int lineStart = (w / 2) - lineLen / 2;
        if (lineStart < 2) lineStart = 2;

        col = lineStart;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (col >= w - 2) break;
            drawManager->SetCharInPosition(line[i], row, col++);
        }
    }

    int escRow = centerY + 3 + (int)games.size() * 2 + 2;
    if (escRow < h - 2)
    {
        char escMsg[] = "(ESC) Salir";
        int escLen = 0;
        for (int i = 0; escMsg[i] != '\0'; i++) escLen++;
        int escStart = (w / 2) - escLen / 2;
        if (escStart < 2) escStart = 2;
        col = escStart;
        for (int i = 0; escMsg[i] != '\0'; i++)
        {
            if (col >= w - 2) break;
            drawManager->SetCharInPosition(escMsg[i], escRow, col++);
        }
    }

    drawManager->PrintMatrix();
}

int GameEngine::ShowMenuAndWaitForSelection()
{
    DrawMenu();

    while (true)
    {
        if (inputManager->IsEscPressed())
            return -1;

        for (size_t i = 0; i < games.size(); i++)
        {
            int vk = (int)games[i].key;
            if (inputManager->IsKeyJustPressed(vk))
                return (int)i;
        }

        Sleep(10);
    }
}

void GameEngine::GameLoop()
{
    bool waitingForInput = false;
    bool returnToMenu = false;

    while (!exitEngine && !returnToMenu)
    {
        if (timeManager->shouldExecuteNextFrame())
        {
            Update();
            Draw();
        }

        if (currentGame->IsGameOver())
        {
            if (inputManager->IsEscPressed())
            {
                exitEngine = true;
            }
            else if (inputManager->IsKeyJustPressed('M'))
            {
                returnToMenu = true;
            }
            else if (!waitingForInput && inputManager->IsRestartKeyPressed())
            {
                currentGame->Reset();
                waitingForInput = true;
            }
            else if (waitingForInput && !inputManager->IsRestartKeyPressed())
            {
                waitingForInput = false;
            }
        }
        else
        {
            if (inputManager->IsEscPressed())
                exitEngine = true;
        }
    }
}

void GameEngine::Update()
{
    const float dt = timeManager->GetElapsedTime();
    currentGame->Update(dt, *inputManager);
}

void GameEngine::Draw()
{
    drawManager->BeginFrame();
    currentGame->Draw(*drawManager);
    drawManager->PrintMatrix();
}