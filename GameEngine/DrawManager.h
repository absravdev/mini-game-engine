#pragma once

constexpr int HUD_ROW_TEXT = 1;
constexpr int HUD_ROW_SEPARATOR = 2;
constexpr int PLAYFIELD_TOP = 3;

class DrawManager
{
private:
    int    height = 1;
    int    width = 1;
    char** matrix = nullptr;
    char   initializeCharacter = ' ';

    DrawManager() {}

public:
    DrawManager(int height, int width);
    ~DrawManager();

    void SetCharInPosition(char c, int h, int w);
    void DrawCharsInLine(char c, int heightLine, int startPos, int endPos);
    void DrawCharsInColumn(char c, int widthColumn, int startPos, int endPos);

    void BeginFrame();
    void PrintMatrix();

    void DrawHUD(int score, int best, bool gameOver);

    int GetHeight() const { return height; }
    int GetWidth()  const { return width; }

private:
    void InitializeMatrix(char c);
    void ClearScreen();
    void DrawBoard();
};