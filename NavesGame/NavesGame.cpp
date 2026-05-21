#include "NavesGame.h"
#include "PlayerShip.h"
#include "DrawManager.h"
#include "InputManager.h"
#include <fstream>
#include <algorithm>
#include <cstdio>

NavesGame::NavesGame(int h, int w, const std::string& xmlFile,
    const std::string& recordFile)
    : height(h), width(w), xmlPath(xmlFile), recordFilePath(recordFile)
{
    LoadRecord();

    if (!waveManager.LoadFromXML(xmlPath))
    {
        //no se encuentra el xtml
    }
}

NavesGame::~NavesGame()
{
    DestroyAllActors();
}

void NavesGame::Reset()
{
    DestroyAllActors();
    pendingAdd.clear();

    score = 0;
    gameOver = false;

    waveManager.Reset();
    waveManager.LoadFromXML(xmlPath);

    player = new PlayerShip(this, PLAYER_SPEED, PLAYER_SHOOT_INTERVAL, PLAYER_CHAR);
    player->SetPosition(3.f, (float)((GetMinY() + GetMaxY()) / 2));
    AddActor(player);
    FlushPendingActors();
}

void NavesGame::Update(float dt, const InputManager& input)
{
    if (gameOver) return;

    player->SetInput(&input);

    waveManager.Update(dt, this, width);

    FlushPendingActors();

    for (auto* actor : actors)
    {
        if (actor->IsAlive())
            actor->Tick(dt);
    }

    // Remove actors out of bounds
    for (auto* actor : actors)
    {
        if (!actor->IsAlive()) continue;
        int ix = actor->GetIntX();
        int iy = actor->GetIntY();
        if (ix < 0 || ix >= width || iy < GetMinY() - 1 || iy > GetMaxY() + 1)
        {
            actor->Destroy();
        }
    }

    CheckCollisions();
    RemoveDeadActors();
    FlushPendingActors();
    RemoveDeadActors();
}

void NavesGame::Draw(DrawManager& dm)
{
    DrawGameHUD(dm);

    for (const auto* actor : actors)
    {
        if (!actor->IsAlive()) continue;
        int x = actor->GetIntX();
        int y = actor->GetIntY();
        if (x > 0 && x < width - 1 && y >= PLAYFIELD_TOP && y < height - 1)
        {
            dm.SetCharInPosition(actor->GetDisplayChar(), y, x);
        }
    }
}

void NavesGame::AddActor(Actor* actor)
{
    if (actor)
    {
        actor->BeginPlay();
        pendingAdd.push_back(actor);
    }
}

void NavesGame::DestroyActor(Actor* actor)
{
    if (actor)
        actor->Destroy();
}

void NavesGame::AddScore(int pts)
{
    score += pts;
    if (score > bestScore)
        bestScore = score;
}

void NavesGame::SetGameOver()
{
    gameOver = true;
    if (score > bestScore)
        bestScore = score;
    SaveRecord();
}

int NavesGame::GetMinY() const { return PLAYFIELD_TOP; }
int NavesGame::GetMaxY() const { return height - 2; }
int NavesGame::GetMinX() const { return 1; }
int NavesGame::GetMaxX() const { return width - 2; }

void NavesGame::DestroyAllActors()
{
    for (auto* actor : actors)
        delete actor;
    actors.clear();

    for (auto* actor : pendingAdd)
        delete actor;
    pendingAdd.clear();

    player = nullptr;
}

void NavesGame::FlushPendingActors()
{
    for (auto* a : pendingAdd)
        actors.push_back(a);
    pendingAdd.clear();
}

void NavesGame::CheckCollisions()
{
    auto itA = actors.begin();
    while (itA != actors.end())
    {
        if (!(*itA)->IsAlive()) { ++itA; continue; }

        auto itB = itA;
        ++itB;
        while (itB != actors.end())
        {
            if (!(*itB)->IsAlive()) { ++itB; continue; }

            if ((*itA)->GetIntX() == (*itB)->GetIntX() &&
                (*itA)->GetIntY() == (*itB)->GetIntY())
            {
                (*itA)->OnCollision(*itB);
                (*itB)->OnCollision(*itA);
            }
            ++itB;
        }
        ++itA;
    }
}

void NavesGame::RemoveDeadActors()
{
    auto it = actors.begin();
    while (it != actors.end())
    {
        if (!(*it)->IsAlive())
        {
            delete* it;
            it = actors.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void NavesGame::DrawGameHUD(DrawManager& dm)
{
    int w = dm.GetWidth();

    for (int i = 1; i < w - 1; ++i)
        dm.SetCharInPosition(' ', HUD_ROW_TEXT, i);

    char left[50], right[50];
    sprintf_s(left, sizeof(left), "PUNTUACION: %d", score);
    sprintf_s(right, sizeof(right), "RECORD: %d", bestScore);

    int col = 3;
    for (int i = 0; left[i] != '\0'; i++)
    {
        if (col >= w - 2) break;
        dm.SetCharInPosition(left[i], HUD_ROW_TEXT, col++);
    }

    int rightLen = 0;
    for (int i = 0; right[i] != '\0'; i++) rightLen++;

    int rightStart = (w - 2) - rightLen;
    if (rightStart < 3) rightStart = 3;

    col = rightStart;
    for (int i = 0; right[i] != '\0'; i++)
    {
        if (col >= w - 2) break;
        dm.SetCharInPosition(right[i], HUD_ROW_TEXT, col++);
    }

    if (gameOver)
    {
        char msg[] = "|GAME OVER: (R) RESET / (M) MENU / (ESC) SALIR|";
        int msgLen = 0;
        for (int i = 0; msg[i] != '\0'; i++) msgLen++;

        int start = (w / 2) - msgLen / 2;
        if (start < 3) start = 3;
        col = start;
        for (int i = 0; msg[i] != '\0'; i++)
        {
            if (col >= w - 2) break;
            dm.SetCharInPosition(msg[i], HUD_ROW_TEXT, col++);
        }
    }
}

void NavesGame::LoadRecord()
{
    std::ifstream file(recordFilePath);
    if (file.is_open())
    {
        file >> bestScore;
        file.close();
    }
}

void NavesGame::SaveRecord()
{
    std::ofstream file(recordFilePath);
    if (file.is_open())
    {
        file << bestScore;
        file.close();
    }
}
