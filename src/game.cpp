#include "game.h"
#include "timer.h"
#include "rect.h"
#include "sdl_functions.h"
#include "helpful_functions.h"
#include <stdio.h>

Game::Game()
{
    background = NULL;
    screen = NULL;
    game_over_screen = NULL;
    main_menu_screen = NULL;
    win_screen = NULL;
    font = NULL;
    retry_button = NULL;
    restart_button = NULL;
    quit_button = NULL;
    start_button = NULL;
    load_map_button = NULL;
    map_editor_button = NULL;
    quit_main_button = NULL;
    player = NULL;
    map = NULL;
    mini_map = NULL;
    map_editor = NULL;
    widgets = NULL;
    cap = true;
    dev_mode = false;

    game_state = MAIN_MENU;

    font = NULL;
}

bool
Game::init()
{
    //Initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }

    int btn_x = SCREEN_WIDTH/2;
    int btn_y = SCREEN_HEIGHT/2;

    retry_button = new Button(btn_x - 75,
            btn_y - 25, 150, 50);
    retry_button->set_off_color(255,255,255);

    restart_button = new Button(btn_x - 75,
            btn_y - 25, 150, 50);
    restart_button->set_off_color(255,255,255);

    quit_button = new Button(btn_x - 75,
            btn_y + 50, 150, 50);
    quit_button->set_off_color(255,255,255);

    start_button = new Button(btn_x - 75,
            btn_y - 100, 150, 50);
    start_button->set_off_color(255,255,255);

    load_map_button = new Button(btn_x - 75,
            btn_y - 25, 150, 50);
    load_map_button->set_off_color(255,255,255);

    map_editor_button = new Button(btn_x - 75,
            btn_y + 50, 150, 50);
    map_editor_button->set_off_color(255,255,255);

    quit_main_button = new Button(btn_x - 75,
            btn_y + 125, 150, 50);
    quit_main_button->set_off_color(255,255,255);

    //Set up the screen
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
            SCREEN_BPP, SDL_SWSURFACE);

    //If there was an error in setting up the screen
    if (screen == NULL) {
        return false;
    }

    map = new Map();
    mini_map = new MiniMap(MINI_MAP_X_BLOCKS, MINI_MAP_Y_BLOCKS, map);
    player = new Player();
    map_editor = new MapEditor();

    widgets = new Widgets();
    widgets->init();

    //Initialize SDL_ttf
    if (!TTF_WasInit()) {
        if (TTF_Init() == -1) {
            return false;
        }
    }

    //Set the window caption
    SDL_WM_SetCaption("MegaCool", NULL);

    //If everything initialized fine
    return true;
}

bool
Game::load_files()
{
    //Load the background image
    background = sdl_func::load_image("img/background.png");
    game_over_screen = sdl_func::load_image("img/game_over.png");
    win_screen = sdl_func::load_image("img/win_screen.png");
    main_menu_screen = sdl_func::load_image("img/main_menu.png");

    font = TTF_OpenFont("data/fonts/arial.ttf", 28);

    if (!player->set_image("img/treeman_running_left.png", "img/treeman_running_right.png",
                "img/treeman_running_left_flipped.png", "img/treeman_running_right_flipped.png")) {
        return false;
    }
    
    if (game_over_screen == NULL) {
        return false;
    }
    
    if (win_screen == NULL) {
        return false;
    }

    if (main_menu_screen == NULL) {
        return false;
    }

    if (background == NULL) {
        return false;
    }

    if (font == NULL) {
        return false;
    }

    if (!retry_button->set_message(font, "Retry")) {
        return false;
    }

    if (!restart_button->set_message(font, "Restart")) {
        return false;
    }

    if (!quit_button->set_message(font, "Quit")) {
        return false;
    }
    
    if (!start_button->set_message(font, "Start")) {
        return false;
    }

    if (!load_map_button->set_message(font, "Load Map")) {
        return false;
    }
    
    if (!map_editor_button->set_message(font, "Map Editor")) {
        return false;
    }

    if (!quit_main_button->set_message(font, "Quit")) {
        return false;
    }

    map->load_file("data/maps/megacool.xml");
    Help_func::dbg_printf("setting player start_x to %d\n", map->get_spawn_x());
    Help_func::dbg_printf("setting player start_y to %d\n", map->get_spawn_y());
    player->set_x_start(map->get_spawn_x());
    player->set_y_start(map->get_spawn_y());
    player->reset();

    /* update mini map when the map is loaded */
    mini_map->update();

    //If everything loaded fine
    return true;
}

/* this should be called after Player::update()
 * has been called. */
bool
Game::collision_detection()
{
    int old_x = player->get_old_x();
    int old_y = player->get_old_y();
    int w = player->get_width();
    int h = player->get_height();
    Help_func::dbg_printf("player old_x=%d, old_y=%d\n", old_x, old_y);

    /* TODO
     * Check if there is overlap with old coordinates.
     * If there is, push out somehow... */

    Rect* player_rect = player->get_rect();

    std::vector<Block*> overlap_objects = 
        map->block_overlap_checks(player_rect);

    std::vector<Block*>::iterator i;
    std::vector<Block*>::iterator end = overlap_objects.end();

    for (i = overlap_objects.begin(); i < end; ++i) {
        Block* block = (*i);

        Help_func::dbg_printf("overlapping with x=%d, y=%d\n", (*i)->get_x(), (*i)->get_y());
        Help_func::dbg_printf("in loop, size = %d\n", overlap_objects.size());

        bool sliding_into = block->sliding_into(old_x, w);
        bool falling_into = block->falling_into(old_y, h);

        if ((falling_into || sliding_into) && block->is_fatal()) {
            /* fell into spikes */
            if (dev_mode) {
                game_state = DEV;
            } else {
                game_state = LOST;
            }
        }

        /* TODO combine to two statements */
        if (falling_into && !sliding_into) {
            Help_func::dbg_printf("a\na\na\na\na\na\na\na\na\n");
            player->set_y_after_crash(block->get_outer_y(old_y));
            player->neutral_gravity = true;
        }

        if (sliding_into) {
            Help_func::dbg_printf("b\nb\nb\nb\nb\nb\nb\nb\nb\n");
            player->set_x_after_crash(block->get_outer_x(old_x));
        }
    }

    if (map->goal_check(player_rect)) {
        game_state = WON;
    }

    if (overlap_objects.size() < 1) {
        Help_func::dbg_printf("neutral_gravity = false;\n");
        player->neutral_gravity = false;
    }

    delete player_rect;

    return true;
}

/* returns true if event.type == SDL_QUIT */
bool
Game::handle_player_events(SDL_Event &event)
{
    //If a key was pressed
    if (event.type == SDL_KEYDOWN) {
        //If enter was pressed
        switch (event.key.keysym.sym) {
            case SDLK_RETURN:
                cap = !cap;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                player->change_dir('r');
                break;
            case SDLK_LEFT:
            case SDLK_a:
                player->change_dir('l');
                break;
            case SDLK_UP:
            case SDLK_w:
                player->change_gravity('u');
                break;
            case SDLK_DOWN:
            case SDLK_s:
                player->change_gravity('d');
                break;
            case SDLK_m:
                /* hotkey only available in dev mode */
                if (dev_mode) {
                    load_map();
                }
                break;
            case SDLK_e:
                /* hotkey only available in dev mode */
                if (dev_mode) {
                    game_state = EDITING_MAP;
                }
                break;
            case SDLK_z:
                dev_mode = !dev_mode;
                break;
            case SDLK_q:
                return true;
                break;
            case SDLK_ESCAPE:
                /* pressing escape will return you to main menu */
                game_state = MAIN_MENU;
                break;
            default:
                break;
        }
    }

    if (event.type == SDL_KEYUP) {
        if (player->is_dir_set()) {
            bool same_key = false;

            switch (event.key.keysym.sym) {
                case SDLK_RIGHT:
                case SDLK_d:
                    if (player->get_dir() == 'r') same_key = true;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if (player->get_dir() == 'l') same_key = true;
                    break;
                default:
                    break;
            }

            if (same_key) {
                player->change_dir('\0');
            }
        }
    }
    //If the user has Xed out the window
    else if (event.type == SDL_QUIT) {
        //Quit the program
        return true;
    }

    return false;
}

bool
Game::handle_mouse_events(SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        /* separate tests to click correct button */
        bool reset = false;
        if (game_state == LOST) {
            reset = retry_button->boundary_chk(event.button.x, event.button.y);
            
            /* quit the game if quit button was pressed */
            if (quit_button->boundary_chk(event.button.x, event.button.y)) {
                return true;
            }
        } else if (game_state == WON) {
            reset = restart_button->boundary_chk(event.button.x, event.button.y);

            /* quit the game if quit button was pressed */
            if (quit_button->boundary_chk(event.button.x, event.button.y)) {
                return true;
            }
        } else if (game_state == MAIN_MENU) {
            reset = start_button->boundary_chk(event.button.x, event.button.y);

            if (load_map_button->boundary_chk(event.button.x, event.button.y)) {
                load_map();
            }

            if (map_editor_button->boundary_chk(event.button.x, event.button.y)) {
                game_state = EDITING_MAP;
            }

            if (quit_main_button->boundary_chk(event.button.x, event.button.y)) {
                return true;
            }
        }

        if (reset) {
            // perhaps reset score or something like that?
            player->reset();
            map->reset_to_spawn_zone();
            mini_map->update();
            game_state = PLAYING;
        }
    } else if (event.type == SDL_QUIT) {
        //Quit the program
        return true;
    }

    return false;
}

bool
Game::zone_control()
{
    /* TODO: might need some kind of return code from zone_control().
     * Need to indicate if player fell to death or object is hindering zone change */
    int x = player->get_x();
    int y = player->get_y();
    int w_d2 = player->get_width() / 2;
    int h_d2 = player->get_height() / 2;
    Map::ZONE_CODE return_code = Map::SAFE;
    //bool zone_set = true;
    Rect* player_rect = NULL;

    if (x + w_d2 > SCREEN_WIDTH) {
        player->restore_y();
        player->set_x(-w_d2);
        player_rect = player->get_rect();
        return_code = map->set_zone_relative(player_rect, 'r');
        mini_map->update();
    } else if (x + w_d2 < 0) {
        player->restore_y();
        player->set_x(SCREEN_WIDTH - w_d2);
        player_rect = player->get_rect();
        return_code = map->set_zone_relative(player_rect, 'l');
        mini_map->update();
    } else if (y + h_d2 > SCREEN_HEIGHT) {
        player->restore_x();
        player->set_y(-h_d2);
        player_rect = player->get_rect();
        return_code = map->set_zone_relative(player_rect, 'd');
        mini_map->update();
    } else if (y + h_d2 < 0) {
        player->restore_x();
        player->set_y(SCREEN_HEIGHT - h_d2);
        player_rect = player->get_rect();
        return_code = map->set_zone_relative(player_rect, 'u');
        mini_map->update();
    }

    switch (return_code) {
        case Map::SAFE:
            /* nothing needs to be done */
            break;
        case Map::OUT_OF_BOUNDS:
            return false;
            break;
        case Map::BLOCKED:
            player->restore_x();
            player->restore_y();
            break;
        case Map::ERROR:
            printf("received ERROR code when switching zone\n");
            SDL_Quit();
            break;
    }

    if (player_rect != NULL) {
        delete player_rect;
    }

    return true;
}

bool
Game::load_map()
{
    Help_func::dbg_printf("loading map\n");

    char* path = NULL;

    widgets->file_dialog(&path, LOAD_FILE);

    if (path == NULL) {
        Help_func::dbg_printf("error loading map\n");
        return false;
    } else {
        map->load_file(path);

        player->set_x_start(map->get_spawn_x());
        player->set_y_start(map->get_spawn_y());
        player->reset();

        mini_map->update();

        delete [] path;
    }

    Help_func::dbg_printf("done loading map\n");

    return true;
}

void
Game::main_menu()
{
    start_button->apply_surface(main_menu_screen);
    load_map_button->apply_surface(main_menu_screen);
    map_editor_button->apply_surface(main_menu_screen);
    quit_main_button->apply_surface(main_menu_screen);

    sdl_func::apply_surface(0,0, main_menu_screen, screen);
}

void
Game::win()
{
    restart_button->apply_surface(win_screen);
    quit_button->apply_surface(win_screen);

    sdl_func::apply_surface(0, 0, win_screen, screen);
}

void
Game::game_over()
{
    retry_button->apply_surface(game_over_screen);
    quit_button->apply_surface(game_over_screen);

    sdl_func::apply_surface(0, 0, game_over_screen, screen);
}

bool
Game::game_loop()
{
    //Quit flag
    bool quit = false;

    //Keep track of the current frame
    int frame = 0;

    //The frame rate regulator
    Timer fps;

    //While the user hasn't quit
    while (!quit) {
        //Start the frame timer
        fps.start();

        //The event structure
        SDL_Event event;
        
        switch (game_state) {
            case MAIN_MENU:
                {
                    while (SDL_PollEvent(&event)) {
                        quit = handle_mouse_events(event);
                    }

                    main_menu();
                }
                break;
            case PLAYING:
            case DEV:
                {
                    while (SDL_PollEvent(&event)) {
                        quit = handle_player_events(event);
                    }

                    player->update();

                    collision_detection();

                    if (!zone_control()) {
                        /* player have fallen out of screen and should die */
                        game_state = LOST;
                        Help_func::dbg_printf("\n\n\nDEAD\n\n\n");
                    }

                    //Apply the background
                    sdl_func::apply_surface(0, 0, background, screen);

                    map->draw_current_zone(screen);
                    player->draw(screen);

                    mini_map->draw(screen);
                }
                break;
            case LOST:
                {
                    while (SDL_PollEvent(&event)) {
                        quit = handle_mouse_events(event);
                    }

                    game_over();
                }
                break;
            case EDITING_MAP:
                {
                    while (SDL_PollEvent(&event)) {
                        if (map_editor->handle_user_events(event)) {
                            /* user quit map editor, return to main menu */
                            game_state = MAIN_MENU;
                        }
                    }

                    //Apply the background
                    sdl_func::apply_surface(0, 0, background, screen);

                    map_editor->draw(screen);
                }
                break;
            case WON:
                {
                    while (SDL_PollEvent(&event)) {
                        quit = handle_mouse_events(event);
                    }

                    win();
                }
                break;
            default:
                break;
        }

        //Update the screen
        if (SDL_Flip(screen) == -1) {
            return false;
        }

        //Increment the frame counter
        frame++;

        //If we want to cap the frame rate
        if ((cap) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)) {
            //Sleep the remaining frame time
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }

    return true;
}

/* TODO:
 * should SDL be initiated and exited in this class? */
Game::~Game()
{
    //Free the surfaces
    SDL_FreeSurface(background);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(game_over_screen);
    SDL_FreeSurface(win_screen);
    SDL_FreeSurface(main_menu_screen);

    TTF_CloseFont(font);

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();

    /* TEMP TOO */
    delete player;
    delete map;
    delete mini_map;
    delete retry_button;
    delete restart_button;
    delete quit_button;
    delete start_button;
    delete load_map_button;
    delete map_editor_button;
    delete quit_main_button;
    delete map_editor;
}
