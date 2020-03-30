//The headers
#include "game.h"
#include "string.h"

int main (int argc, char* argv[])
{
    Game *game = new Game();

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "dbg") == 0 || strcmp(argv[i], "debug") == 0) {
            Help_func::set_debug(true);
        }
    }

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