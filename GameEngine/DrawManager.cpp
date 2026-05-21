#include "DrawManager.h"
#include <iostream>
#include <string>
#include <windows.h>

DrawManager::DrawManager(int height, int width)
    : height(height), width(width)
{
    matrix = new char* [height];
    for (int i = 0; i < height; i++)
        matrix[i] = new char[width];

    InitializeMatrix(initializeCharacter);
    DrawBoard();
}

DrawManager::~DrawManager()
{
    if (matrix)
    {
        for (int i = 0; i < height; i++)
            delete[] matrix[i];
        delete[] matrix;
        matrix = nullptr;
    }
}

void DrawManager::InitializeMatrix(char c)
{
    if (!matrix) return;
    for (int h = 0; h < height; h++)
        for (int w = 0; w < width; w++)
            matrix[h][w] = c;
}

void DrawManager::ClearScreen()
{
    static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD home = { 0, 0 };
    SetConsoleCursorPosition(h, home);
}

void DrawManager::PrintMatrix()
{
    ClearScreen();
    if (!matrix) return;

    std::string frame;
    frame.reserve((size_t)height * (size_t)(width + 1));

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
            frame.push_back(matrix[h][w]);
        frame.push_back('\n');
    }

    std::cout << frame;
}

void DrawManager::SetCharInPosition(char c, int h, int w)
{
    if (!matrix) return;
    if (h < 0 || h >= height) return;
    if (w < 0 || w >= width)  return;
    matrix[h][w] = c;
}

void DrawManager::DrawCharsInLine(char c, int heightLine, int startPos, int endPos)
{
    if (!matrix) return;
    if (heightLine < 0 || heightLine >= height) return;
    if (startPos > endPos) return;
    if (startPos < 0)         startPos = 0;
    if (endPos > width - 1)   endPos = width - 1;

    for (int i = startPos; i <= endPos; i++)
        matrix[heightLine][i] = c;
}

void DrawManager::DrawCharsInColumn(char c, int widthColumn, int startPos, int endPos)
{
    if (!matrix) return;
    if (widthColumn < 0 || widthColumn >= width) return;
    if (startPos > endPos) return;
    if (startPos < 0)          startPos = 0;
    if (endPos > height - 1)   endPos = height - 1;

    for (int i = startPos; i <= endPos; i++)
        matrix[i][widthColumn] = c;
}

void DrawManager::DrawBoard()
{
    SetCharInPosition((char)201, 0, 0);
    SetCharInPosition((char)187, 0, width - 1);
    SetCharInPosition((char)200, height - 1, 0);
    SetCharInPosition((char)188, height - 1, width - 1);

    DrawCharsInLine((char)205, 0, 1, width - 2);
    DrawCharsInLine((char)205, height - 1, 1, width - 2);

    DrawCharsInColumn((char)186, 0, 1, height - 2);
    DrawCharsInColumn((char)186, width - 1, 1, height - 2);

    SetCharInPosition((char)204, HUD_ROW_SEPARATOR, 0);
    SetCharInPosition((char)185, HUD_ROW_SEPARATOR, width - 1);
    DrawCharsInLine((char)205, HUD_ROW_SEPARATOR, 1, width - 2);
}

void DrawManager::BeginFrame()
{
    for (int h = PLAYFIELD_TOP; h < height - 1; h++)
        for (int w = 1; w < width - 1; w++)
            matrix[h][w] = initializeCharacter;
}

void DrawManager::DrawHUD(int score, int best, bool gameOver)
{
    for (int w = 1; w < width - 1; ++w)
        matrix[HUD_ROW_TEXT][w] = ' ';

    char left[50], right[50];
    sprintf_s(left, sizeof(left), "SCORE: %d", score);
    sprintf_s(right, sizeof(right), "BEST: %d", best);

    int col = 3;
    for (int i = 0; left[i] != '\0'; i++)
    {
        if (col >= width - 2) break;
        SetCharInPosition(left[i], HUD_ROW_TEXT, col++);
    }

    int rightLen = 0;
    for (int i = 0; right[i] != '\0'; i++) rightLen++;

    int rightStart = (width - 2) - rightLen;
    if (rightStart < 3) rightStart = 3;

    col = rightStart;
    for (int i = 0; right[i] != '\0'; i++)
    {
        if (col >= width - 2) break;
        SetCharInPosition(right[i], HUD_ROW_TEXT, col++);
    }

    if (gameOver)
    {
        char msg[] = "GAME OVER: PLAY AGAIN (R) EXIT (ESC)";
        int msgLen = 0;
        for (int i = 0; msg[i] != '\0'; i++) msgLen++;

        int start = (width / 2) - msgLen / 2;
        if (start < 3) start = 3;
        col = start;
        for (int i = 0; msg[i] != '\0'; i++)
        {
            if (col >= width - 2) break;
            SetCharInPosition(msg[i], HUD_ROW_TEXT, col++);
        }
    }
}