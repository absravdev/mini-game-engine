#include "GameEngine.h"
#include "NavesGame.h"

int main()
{
    const int fps = 100;
    const int height = 20;
    const int width = 80;

    NavesGame game(height, width, "waves.xml", "naves_record.txt");

    GameEngine engine(fps, height, width);

    engine.RegisterGame('N', "Naves", &game);

    engine.Start();

    return 0;
}
