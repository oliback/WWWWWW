#ifndef ZONE_H
#define ZONE_H

#include "rect.h"
#include "block.h"
#include "spikes.h"

#include <vector>

class Zone
{
    private:
        int x, y;
        std::vector<Block*> blocks;
        std::vector<Spikes*> spikes;
        /* vectors for all objects */
    public:
        Zone();
        ~Zone();

        int get_x();
        int get_y();

        /* Map editor needs to access this but it doesn't
         * feel right to allow public access to blocks/spikes */
        std::vector<Block*>::iterator get_blocks_begin();
        std::vector<Block*>::iterator get_blocks_end();
        std::vector<Spikes*>::iterator get_spikes_begin();
        std::vector<Spikes*>::iterator get_spikes_end();

        void set_xy(int x, int y);
        void add_block(Block *block);
        void add_spikes(Spikes* spikes);
        bool is_xy_same(int x, int y);
        /* eventual update function (moving objects) */
        void draw(SDL_Surface *dest);

        std::vector<Block*> overlap_checks(Rect* r);
};

#endif
