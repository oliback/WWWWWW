//The headers
#include "game.h"

int main (int argc, char* args[])
{
    Game *game = new Game();

    //Initialize
    if (game->init() == false) {
        printf("main.cpp: game initialization failed\n");
        return 1;
    }

    //Load the files
    if (game->load_files() == false) {
        printf("main.cpp: game loading files failed\n");
        return 1;
    }

    if (game->game_loop() == false) {
        printf("main.cpp: game loop failed\n");
        return 1;
    }

    delete game;

    return 0;
}
