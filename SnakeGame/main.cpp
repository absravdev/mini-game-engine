#include "GameEngine.h"
#include "SnakeGame.h"

int main()
{
    const int fps = 100;
    const int height = 20;
    const int width = 80;

    SnakeGame game(height, width);

    GameEngine engine(fps, height, width);

    engine.RegisterGame('S', "Snake", &game);

    engine.Start();

    return 0;
}