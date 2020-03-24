#include "mini_map.h"

MiniMap::MiniMap(int x_block_count, int y_block_count, Map* map)
{
    this->x_block_count = x_block_count;
    this->y_block_count = y_block_count;
    this->zone_block_count = x_block_count * y_block_count;
    zone_blocks = new Block*[zone_block_count];
    this->map = NULL;
    this->map = map;

    // light gray
    base_color.r = 112;
    base_color.g = 112;
    base_color.b = 112;

    // darker gray
    zone_color.r = 77;
    zone_color.g = 77;
    zone_color.b = 77;

    // green
    player_color.r = 0;
    player_color.g = 255;
    player_color.b = 0;

    goal_color.r = 255;
    goal_color.g = 215;
    goal_color.b = 0;

    int x_padding = 4;
    int y_padding = 4;
    int block_width = 16;
    int block_height = 16;
    int opacity = 160;

    base_block = new Block(x_padding, y_padding,
            (x_block_count + 1) * x_padding + x_block_count * block_width,
            (y_block_count + 1) * y_padding + y_block_count * block_height);
    base_block->set_color(base_color.r, base_color.g, base_color.b);

    base_block->set_alpha(opacity);

    int max_block_x = base_block->get_width() - x_padding;

    for (int i = 0; i < zone_block_count; ++i)
    {
        int y_threshold = (int)(i / y_block_count);

        zone_blocks[i] = new Block(
                x_padding + ((x_padding * (i + 1) + block_width * i) % max_block_x),
                y_padding + (y_padding * (y_threshold + 1)) + (block_height * y_threshold),
                block_width,
                block_height);

        zone_blocks[i]->set_color(zone_color.r, zone_color.g, zone_color.b);

        zone_blocks[i]->set_alpha(opacity);
    }

    /* set middle block to green for showing player color */
    if (x_block_count % 2 == 0 || y_block_count % 2 == 0) {
        printf("warning: mini map zone count does not have a middle\n");
    }

    int middle = zone_block_count / 2;
    zone_blocks[middle]->set_color(player_color.r, player_color.g, player_color.b);
}

MiniMap::~MiniMap()
{
    /* don't delete map since that is not ours */

    delete base_block;

    for (int i = 0; i < zone_block_count; ++i)
    {
        delete zone_blocks[i];
        zone_blocks[i] = NULL;
    }

    delete [] zone_blocks;
}

void
MiniMap::set_block_off(int x, int y)
{
    zone_blocks[y * y_block_count + x]->set_color(
            base_color.r,
            base_color.g,
            base_color.b);
}

void
MiniMap::set_block_on(int x, int y)
{
    zone_blocks[y * y_block_count + x]->set_color(
            zone_color.r,
            zone_color.g,
            zone_color.b);
}

void
MiniMap::update()
{
    int mini_map_x;
    int mini_map_y;
    int map_x;
    int map_y;

    int player_x = map->get_current_zone_x();
    int player_y = map->get_current_zone_y();

    int start_x = player_x - (int)(x_block_count / 2);
    int start_y = player_y - (int)(y_block_count / 2);

    for (int i = 0; i < zone_block_count; ++i)
    {
        mini_map_x = (i % x_block_count); 
        mini_map_y = ((int)(i / y_block_count)); 

        map_x = start_x + mini_map_x;
        map_y = start_y + mini_map_y;

        if (map->zone_exists(map_x, map_y)) {
            if (map->is_goal_zone(map_x, map_y)) {
                zone_blocks[mini_map_y * y_block_count + mini_map_x]->set_color(
                        goal_color.r,
                        goal_color.g,
                        goal_color.b);
            } else {
                set_block_on(mini_map_x, mini_map_y);
            }
        } else {
            set_block_off(mini_map_x, mini_map_y);
        }
    }

    /* set middle block to player color again */
    int middle = ((int)(y_block_count / 2)) * y_block_count + ((int)(x_block_count / 2));
    //int middle = zone_block_count % 2 == 0 ? 
    zone_blocks[middle]->set_color(
            player_color.r,
            player_color.g,
            player_color.b);
}

void
MiniMap::draw(SDL_Surface* surface)
{
    base_block->apply_surface(surface);

    for (int i = 0; i < zone_block_count; ++i)
    {
        zone_blocks[i]->apply_surface(surface);
    }
}
