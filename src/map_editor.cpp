#include "map_editor.h"
#include "sdl_functions.h"
#include "rect.h"
#include "spikes.h"
#include <time.h>

MapEditor::MapEditor()
{
    map = new Map();
    map->new_zones();
    mini_map = new MiniMap(MINI_MAP_X_BLOCKS, MINI_MAP_Y_BLOCKS, map);
    mini_map->update();
    draw_block = NULL;
    active_tool = NONE;
    snap_active = true;
    widgets = new Widgets();
    widgets->init();

    draw_color.r = rand_color_component();
    draw_color.g = rand_color_component();
    draw_color.b = rand_color_component();

    int y = SCREEN_HEIGHT - 60;
    int width = 40;
    int height = 40;
    int x_padding = 8;

    /* Button(x, y, width, height, r, g, b) */
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        buttons[i] = new Button(x_padding * (i + 1) + i * width, y, width, height);
        buttons[i]->set_off_color(10, 105, 163); //blue
        buttons[i]->set_on_color(0, 255, 0); //green
        buttons[i]->set_text_color(0, 0, 0);
        buttons[i]->set_alpha(160);
    }

    /* initialize TTF if it is not done already */
    if (!TTF_WasInit()) {
        if (TTF_Init() == -1) {
            //printf("TTF_Init() returned -1 in MapEditor::MapEditor()\n");
        }
    }

    font = TTF_OpenFont("data/fonts/arial.ttf", 9);

    if (font == NULL) {
        //printf("invalid font argument in MapEditor::Mapeditor()\n");
        SDL_Quit();
    }

    buttons[0]->set_message(font, "BLOCK");
    buttons[0]->set_tag((int)BLOCK_BUTTON);

    buttons[1]->set_message(font, "SPIKES");
    buttons[1]->set_tag((int)SPIKES_BUTTON);

    buttons[2]->set_message(font, "ROTATE");
    buttons[2]->set_tag((int)ROTATE_BUTTON);

    buttons[3]->set_message(font, "LEFT");
    buttons[3]->set_tag((int)LEFT_ZONE_BUTTON);

    buttons[4]->set_message(font, "RIGHT");
    buttons[4]->set_tag((int)RIGHT_ZONE_BUTTON);

    buttons[5]->set_message(font, "DOWN");
    buttons[5]->set_tag((int)DOWN_ZONE_BUTTON);

    buttons[6]->set_message(font, "UP");
    buttons[6]->set_tag((int)UP_ZONE_BUTTON);

    buttons[7]->set_message(font, "SPAWN");
    buttons[7]->set_tag((int)SPAWN_BUTTON);

    buttons[8]->set_message(font, "GOAL");
    buttons[8]->set_tag((int)GOAL_BUTTON);

    buttons[9]->set_message(font, "SNAP");
    buttons[9]->set_tag((int)SNAP_BUTTON);
    if (snap_active) {
        buttons[9]->flip_on_off();
    }

    buttons[10]->set_message(font, "COLOR");
    buttons[10]->set_tag((int)COLOR_BUTTON);

    buttons[11]->set_message(font, "LOAD");
    buttons[11]->set_tag((int)LOAD_BUTTON);

    buttons[12]->set_message(font, "SAVE");
    buttons[12]->set_tag((int)SAVE_BUTTON);

    srand(time(NULL));
}

MapEditor::~MapEditor()
{
    if (TTF_WasInit() && font != NULL) {
        TTF_CloseFont(font);
    }

    /* delete draw_block if it has not been saved */
    if (draw_block != NULL) {
        delete draw_block;
    }

    delete map;

    for (int i = 0; i < BUTTON_COUNT; ++i) {
        delete buttons[i];
        buttons[i] = NULL;
    }

    delete widgets;
}

int
MapEditor::rand_color_component()
{
    return rand() % 256;
}

void
MapEditor::color_widget()
{
    //COLOR_PICKER is from DIALOG_TYPE enum in widgets.h
    widgets->color_picker(draw_color, COLOR_PICKER);
}

void
MapEditor::save_widget()
{
    char* path = NULL;
    //SAVE_FILE is from DIALOG_TYPE enum in widgets.h
    widgets->file_dialog(&path, SAVE_FILE);

    if (path != NULL) {
        map->save_to_file(path);
        printf("map saved as: %s\n", path);

        delete [] path;
        path = NULL;
    } else {
        printf("map_editor: no file path specified, did not save\n");
    }
}

void
MapEditor::load_widget()
{
    char* path = NULL;
    //LOAD_FILE is from DIALOG_TYPE enum in widgets.h
    widgets->file_dialog(&path, LOAD_FILE);

    if (path != NULL) {
        map->load_file(path);

        mini_map->update();

        printf("loaded: %s\n", path);

        delete [] path;
        path = NULL;
    } else {
        printf("map_editor: did not succeed in loading map\n");
    }

}

int
MapEditor::snap_to_grid(int val)
{
    if (snap_active) {
        if (val % INCREMENT > INCREMENT/2) {
            return (int)(val / INCREMENT) * INCREMENT + INCREMENT;
        } else {
            return (int)(val / INCREMENT) * INCREMENT;
        }
        //return ((int)val / INCREMENT) * INCREMENT;
    } else {
        return val;
    }
}

void
MapEditor::flip_button_color(int button_i)
{
    buttons[button_i]->flip_on_off();
}

void
MapEditor::draw_buttons(SDL_Surface* surface)
{
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        buttons[i]->apply_surface(surface);
    }
}

/* returns true if user wants to quit, false otherwise */
bool
MapEditor::handle_user_events(SDL_Event &event)
{
    /* if a user quits from map editor, he should be returned to main menu in game */
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_q:
            case SDLK_ESCAPE:
                /* quit from map editor */
                return true;
                break;
            default:
                break;
        }
    } else if (event.type == SDL_QUIT) {
        /* Quit the program */
        return true;
    }

    if (event.type == SDL_MOUSEMOTION) {
        int snap_x = snap_to_grid(event.motion.x);
        int snap_y = snap_to_grid(event.motion.y);

        if (draw_block != NULL) {
            if (active_tool == BLOCK || active_tool == SPIKES) {
                int mouse_w = draw_block->get_x_origin() - snap_x;
                int mouse_h = draw_block->get_y_origin() - snap_y;
                
                if (mouse_w < 0) {
                    /* cursor is to the right of x origin */
                    draw_block->restore_x();
                    draw_block->set_width(-mouse_w);
                } else {
                    /* cursor is to the left of x origin */
                    draw_block->set_x(snap_x);
                    draw_block->set_width(mouse_w);
                }

                if (mouse_h < 0) {
                    /* cursor is under y origin */
                    draw_block->restore_y();
                    draw_block->set_height(-mouse_h);
                } else {
                    /* cursor is over y origin */
                    draw_block->set_y(snap_y);
                    draw_block->set_height(mouse_h);
                }
            } else if (active_tool == SPAWN || active_tool == GOAL) {
                /* the texture should just follow 
                 * the cursor until mousebtnup */
                draw_block->set_x(snap_x);
                draw_block->set_y(snap_y);
            }

            /* limit spikes to their dimensions */
            if (active_tool == SPIKES) {
                int surface_w = draw_block->get_surface_width();
                int surface_h = draw_block->get_surface_height();
                draw_block->set_width(std::max(((int)(draw_block->get_width() / surface_w) * surface_w - 1), 0));
                draw_block->set_height(std::max(((int)(draw_block->get_height() / surface_h) * surface_h - 1), 0));
            }
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_RIGHT) {
            int button_i = -1;
            switch (active_tool) {
                case BLOCK:
                    button_i = (int)BLOCK_BUTTON;
                    if (draw_block != NULL) {
                        delete draw_block;
                        draw_block = NULL;
                    }
                    break;
                case SPIKES:
                    button_i = (int)SPIKES_BUTTON;
                    if (draw_block != NULL) {
                        delete ((Spikes*)draw_block);
                        draw_block = NULL;
                    }
                    break;
                case ROTATE:
                    button_i = (int)ROTATE_BUTTON;
                    break;
                case SPAWN:
                    button_i = (int)SPAWN_BUTTON;
                    if (draw_block != NULL) {
                        delete ((MapPoint*)draw_block);
                        draw_block = NULL;
                    }
                    break;
                case GOAL:
                    button_i = (int)GOAL_BUTTON;
                    if (draw_block != NULL) {
                        delete ((MapPoint*)draw_block);
                        draw_block = NULL;
                    }
                    break;
                default:
                    break;
            }

            if (button_i > -1) {
                flip_button_color(button_i);
            }

            active_tool = NONE;
        }

        /* start the creation of relevant object */
        int snap_x = snap_to_grid(event.button.x);
        int snap_y = snap_to_grid(event.button.y);

        switch (active_tool) {
            case NONE:
                /* No tool is active, check if a button was pressed */
                {
                    /* check if a tool button was pressed */
                    for (int i = 0; i < BUTTON_COUNT; ++i)
                    {
                        if (buttons[i]->boundary_chk(event.button.x, event.button.y)) {
                            switch (buttons[i]->get_tag()) {
                                case BLOCK_BUTTON:
                                    //printf("block button was pressed!\n");
                                    active_tool = BLOCK;
                                    flip_button_color((int)BLOCK_BUTTON);
                                    break;
                                case SPIKES_BUTTON:
                                    active_tool = SPIKES;
                                    flip_button_color((int)SPIKES_BUTTON);
                                    break;
                                case ROTATE_BUTTON:
                                    active_tool = ROTATE;
                                    flip_button_color((int)ROTATE_BUTTON);
                                    break;
                                case LEFT_ZONE_BUTTON:
                                    map->add_new_zone_relative('l');
                                    map->force_zone_relative('l');
                                    mini_map->update();
                                    active_tool = NONE;
                                    break;
                                case RIGHT_ZONE_BUTTON:
                                    map->add_new_zone_relative('r');
                                    map->force_zone_relative('r');
                                    mini_map->update();
                                    active_tool = NONE;
                                    break;
                                case DOWN_ZONE_BUTTON:
                                    map->add_new_zone_relative('d');
                                    map->force_zone_relative('d');
                                    mini_map->update();
                                    active_tool = NONE;
                                    break;
                                case UP_ZONE_BUTTON:
                                    map->add_new_zone_relative('u');
                                    map->force_zone_relative('u');
                                    mini_map->update();
                                    active_tool = NONE;
                                    break;
                                case SPAWN_BUTTON:
                                    active_tool = SPAWN;
                                    flip_button_color((int)SPAWN_BUTTON);
                                    break;
                                case GOAL_BUTTON:
                                    active_tool = GOAL;
                                    flip_button_color((int)GOAL_BUTTON);
                                    break;
                                case SNAP_BUTTON:
                                    snap_active = !snap_active;
                                    flip_button_color((int)SNAP_BUTTON);
                                    break;
                                case COLOR_BUTTON:
                                    color_widget();
                                    break;
                                case LOAD_BUTTON:
                                    printf("map_editor: loading map!\n");
                                    load_widget();
                                    break;
                                case SAVE_BUTTON:
                                    printf("map_editor: saving map!\n");
                                    save_widget();
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
                break;
            case BLOCK:
                //printf("creating new block!\n");
                draw_block = new Block(snap_x, snap_y, 0, 0);
                draw_block->set_color(draw_color.r, draw_color.g, draw_color.b);
                break;
            case SPIKES:
                draw_block = new Spikes(snap_x, snap_y, 0, 0);
                break;
            case ROTATE:
                /* find which spike was pressed, rotate texture */
                {
                    printf("in ROTATE function\n");
                    Rect r(event.button.x, event.button.y);
                    std::vector<Spikes*> overlap_spikes = map->spikes_overlap_checks(&r);

                    std::vector<Spikes*>::iterator i;
                    std::vector<Spikes*>::iterator end = overlap_spikes.end();

                    for (i = overlap_spikes.begin(); i < end; ++i)
                    {
                        printf("found block and I am rotating it\n");
                        (*i)->rotate();
                    }
                }
                break;
            case SPAWN:
                map->replace_spawn(NULL); // to remove the old spawn
                draw_block = new MapPoint(snap_x, snap_y);
                ((MapPoint*)draw_block)->load_spawn_texture();
                break;
            case GOAL:
                map->replace_goal(NULL); // to remove the old goal
                draw_block = new MapPoint(snap_x, snap_y);
                ((MapPoint*)draw_block)->load_goal_texture();
                break;
            default:
                break;
        }

    } else if (event.type == SDL_MOUSEBUTTONUP) {
        /* save the block etc. */
        switch (active_tool) {
            case BLOCK:
                /* to handle when the button is pressed */
                if (draw_block != NULL) {
                    printf("saving block\n");
                    map->add_block_to_current(draw_block);
                    draw_block = NULL;
                }
                break;
            case SPIKES:
                if (draw_block != NULL) {
                    map->add_spikes_to_current((Spikes*)draw_block);
                    draw_block = NULL;
                }
                break;
            case SPAWN:
                if (draw_block != NULL) {
                    ((MapPoint*)draw_block)->set_zone_x(map->get_current_zone_x());
                    ((MapPoint*)draw_block)->set_zone_y(map->get_current_zone_y());
                    map->replace_spawn((MapPoint*)draw_block);
                    draw_block = NULL;
                }
                break;
            case GOAL:
                if (draw_block != NULL) {
                    ((MapPoint*)draw_block)->set_zone_x(map->get_current_zone_x());
                    ((MapPoint*)draw_block)->set_zone_y(map->get_current_zone_y());
                    map->replace_goal((MapPoint*)draw_block);
                    draw_block = NULL;
                }
                break;
            default:
                break;
        }
    }

    return false;
}

bool
MapEditor::draw(SDL_Surface* screen)
{
    map->draw_current_zone(screen);
    map->draw_spawn(screen);

    draw_buttons(screen);

    mini_map->draw(screen);

    /* draw the block object currently being modified */
    if (active_tool != NONE && draw_block != NULL) {
        draw_block->apply_surface(screen);
    }

    return true;
}
