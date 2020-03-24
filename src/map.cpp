#include "map.h"
#include "stdio.h"

Map::Map()
{
    current_zone = NULL;
    io = new IOMap();
    spawn = NULL;
    goal = NULL;

    x = y = 0;
}

Map::~Map()
{
    std::vector<Zone*>::iterator i;
    std::vector<Zone*>::iterator end = zones.end();

    for (i = zones.begin(); i < end; ++i)
    {
        delete (*i);
    }

    zones.clear();

    if (io != NULL) {
        delete io;
    }

    if (spawn != NULL) {
        delete spawn;
    }

    if (goal != NULL) {
        delete goal;
    }
}

bool
Map::change_coords_relative(int& new_x, int& new_y, char dir)
{
    switch (dir) {
        case 'l':
            new_x--;
            break;
        case 'r':
            new_x++;
            break;
        case 'd':
            new_y++;
            break;
        case 'u':
            new_y--;
            break;
        default:
            printf("wrong dir argument received in Map::change_coords_relative\n");
            return false;
            break;
    }

    return true;
}

void
Map::new_zones()
{
    x = y = 0;
    current_zone = new Zone(); //default x = 0, y = 0
    zones.push_back(current_zone);
}

void
Map::load_file(const char* path)
{
    /* clear all zones before loading */
    if (!zones.empty()) {
        std::vector<Zone*>::iterator i;
        std::vector<Zone*>::iterator end = zones.end();

        for (i = zones.begin(); i < end; ++i)
        {
            delete (*i);
        }

        zones.clear();
    }

    if (!io->load_map(path, this)) {
        printf("Map::load_file returned false\n");
        return;
    }

    x = spawn->get_zone_x();
    y = spawn->get_zone_y();

    if (!force_zone(x, y)) {
        printf("Map::force_zone in load_file returned false\n");
    }
}

bool
Map::copy_file(const char* src, const char* dest)
{
    FILE *in = NULL;
    FILE *out = NULL;
    int c;

    in = fopen(src, "r");
    out = fopen(dest, "w");

    if (in == NULL) {
        //g_logger->errorf("%s: Unable to open file for reading", src);
        return false;
    } else if (out == NULL) {
        //g_logger->errorf("%s: Unable to open file for writing", dest);
        return false;
    }

    while ((c = getc(in)) != EOF) {
        putc(c,out);
    }

    fclose(in);
    fclose(out);

    return true;
}

bool
Map::save_to_file(const char* path)
{
    return io->save_map(path, this);
}

void
Map::draw_current_zone(SDL_Surface* dest)
{
    current_zone->draw(dest);

    if (goal != NULL) {
        if (is_goal_zone(current_zone)) {
            goal->apply_surface(dest);
        }
    }
}

void
Map::draw_spawn(SDL_Surface* dest)
{
    if (spawn != NULL) {
        if (is_spawn_zone(current_zone)) {
            spawn->apply_surface(dest);
        }
    }
}

std::vector<Block*>
Map::block_overlap_checks(Rect* r)
{
    return current_zone->overlap_checks(r);
}

std::vector<Spikes*>
Map::spikes_overlap_checks(Rect* r)
{
    std::vector<Block*> overlap_blocks = current_zone->overlap_checks(r);
    std::vector<Spikes*> overlap_spikes;

    std::vector<Block*>::iterator i;
    std::vector<Block*>::iterator end = overlap_blocks.end();

    for (i = overlap_blocks.begin(); i < end; ++i) 
    {
        if ((*i)->is_fatal()) {
            overlap_spikes.push_back((Spikes*)(*i));
        }
    }

    return overlap_spikes;
}

/*
typedef enum ZONE_CODE {
    SAFE, OUT_OF_BOUNDS, BLOCKED, EMPTY
} ZONE_CODE;
    */
Map::ZONE_CODE
Map::set_zone_relative(Rect* player_rect, char dir)
{
    int new_x = x;
    int new_y = y;

    change_coords_relative(new_x, new_y, dir);

    return set_zone(player_rect, new_x, new_y);
}

/*
typedef enum ZONE_CODE {
    SAFE, OUT_OF_BOUNDS, BLOCKED, EMPTY
} ZONE_CODE;
*/
Map::ZONE_CODE
Map::set_zone(Rect* player_rect, int new_x, int new_y)
{
    if (zones.empty()) {
        return ERROR;
    }

    {
        std::vector<Zone*>::iterator i;
        std::vector<Zone*>::iterator end = zones.end();
        for (i=zones.begin(); i < end; ++i) {
            if ((*i)->is_xy_same(new_x, new_y)) {
                /* check that player is not colliding with blocks in the new zone */
                std::vector<Block*> overlap_objects = (*i)->overlap_checks(player_rect);

                if (overlap_objects.size() < 1) {
                    /* player is not overlapping objects in next zone */
                    x = new_x;
                    y = new_y;

                    if (current_zone == (*i)) {
                        return SAFE;
                    }

                    current_zone = *i;

                    printf("switched zone fine\n");
                    return SAFE;
                } else {
                    printf("overlapping object found when switching zone!\n");
                    /* player is overlapping an objects in the next zone and
                     * should not be allowed to enter. */
                    return BLOCKED;
                }
            }
        }
    }

    return OUT_OF_BOUNDS;
}

int
Map::get_current_zone_x()
{
    return current_zone->get_x();
}

int
Map::get_current_zone_y()
{
    return current_zone->get_y();
}

bool
Map::force_zone(int new_x, int new_y)
{
    if (zones.empty()) {
        return false;
    }

    {
        std::vector<Zone*>::iterator i;
        std::vector<Zone*>::iterator end = zones.end();
        for (i=zones.begin(); i < end; ++i) {
            if ((*i)->is_xy_same(new_x, new_y)) {
                x = new_x;
                y = new_y;

                if (current_zone == (*i)) {
                    return true;
                }

                current_zone = *i;

                return true;
            }
        }
    }

    return false;
}

void
Map::add_new_zone_relative(char dir)
{
    int new_x = x;
    int new_y = y;

    change_coords_relative(new_x, new_y, dir);

    Zone* zone = new Zone();
    zone->set_xy(new_x, new_y);

    zones.push_back(zone);
}

bool
Map::force_zone_relative(char dir)
{
    int new_x = x;
    int new_y = y;

    change_coords_relative(new_x, new_y, dir);

    return force_zone(new_x, new_y);
}

void
Map::add_block_to_current(Block* block)
{
    current_zone->add_block(block);
}

void
Map::add_spikes_to_current(Spikes* spikes)
{
    current_zone->add_spikes(spikes);
}

void
Map::print_xy()
{
    printf("Map::x = %d, y = %d\n", x, y);
}

void
Map::create_spawn(int x, int y)
{
    if (spawn != NULL) {
        delete spawn;
    }

    spawn = new MapPoint(x, y);
    spawn->load_spawn_texture();
}

void
Map::create_goal(int x, int y)
{
    if (goal != NULL) {
        delete goal;
    }

    goal = new MapPoint(x, y);
    goal->load_goal_texture();
}

void
Map::replace_spawn(MapPoint* new_spawn)
{
    /* NULL is an allowed argument to reset spawn */
    if (this->spawn != NULL) {
        delete this->spawn;
        this->spawn = NULL;
    }

    this->spawn = new_spawn;
}

void
Map::replace_goal(MapPoint* new_goal)
{
    /* NULL is an allowed argument to reset goal */
    if (this->goal != NULL) {
        delete this->goal;
        this->goal = NULL;
    }

    this->goal = new_goal;
}

void
Map::reset_to_spawn_zone()
{
    force_zone(spawn->get_zone_x(), spawn->get_zone_y());
}

bool
Map::zone_exists(int x_chk, int y_chk)
{
    std::vector<Zone*>::iterator i;
    std::vector<Zone*>::iterator end = zones.end();

    for (i = zones.begin(); i < end; ++i)
    {
        if ((*i)->is_xy_same(x_chk, y_chk)) {
            return true;
        }
    }

    return false;
}

bool
Map::goal_check(Rect* rect)
{
    /* check if player reached goal */
    if (goal != NULL) {
        if (is_goal_zone(current_zone) && goal->overlap(rect)) {
            printf("returning goal\n");
            return true;
        }
    }
    
    return false;
}

bool
Map::is_spawn_zone(Zone* zone)
{
    if (zone == NULL) {
        return false;
    }

    if (spawn == NULL) {
        return false;
    }

    return (zone->get_x() == spawn->get_zone_x() &&
            zone->get_y() == spawn->get_zone_y());
}

bool
Map::is_spawn_zone(int x_chk, int y_chk)
{
    if (spawn == NULL) {
        return false;
    }

    return spawn->get_zone_x() == x_chk &&
        spawn->get_zone_y() == y_chk;
}

bool
Map::is_goal_zone(Zone* zone)
{
    if (zone == NULL) {
        return false;
    }

    if (goal == NULL) {
        return false;
    }

    return (zone->get_x() == goal->get_zone_x() &&
            zone->get_y() == goal->get_zone_y());
}

bool
Map::is_goal_zone(int x_chk, int y_chk)
{
    if (goal == NULL){
        return false;
    }

    return goal->get_zone_x() == x_chk &&
        goal->get_zone_y() == y_chk;
}

int
Map::get_spawn_x()
{
    return spawn->get_x();
}

int
Map::get_spawn_y()
{
    return spawn->get_y();
}
