#include "GameEngine.h"
#include "LluviaGame.h"
#include "SnakeGame.h"
#include "NavesGame.h"

int main()
{
    const int fps = 100;
    const int height = 20;
    const int width = 80;

    LluviaGame lluvia(height, width);
    SnakeGame  snake(height, width);
    NavesGame  naves(height, width, "waves.xml", "naves_record.txt");

    GameEngine engine(fps, height, width);

    engine.RegisterGame('L', "Lluvia de Letras", &lluvia);
    engine.RegisterGame('S', "Snake", &snake);
    engine.RegisterGame('N', "Naves", &naves);

    engine.Start();

    return 0;
}