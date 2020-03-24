#include "zone.h"

Zone::Zone()
{
    x = 0;
    y = 0;
}

Zone::~Zone()
{
    std::vector<Block*>::iterator i;
    std::vector<Block*>::iterator end_block = blocks.end();

    for (i = blocks.begin(); i < end_block; ++i)
    {
        delete (*i);
    }

    std::vector<Spikes*>::iterator j;
    std::vector<Spikes*>::iterator end_spikes;

    for (j = spikes.begin(); j < end_spikes; ++j)
    {
        delete (*j);
    }

    blocks.clear();
    spikes.clear();
}

int
Zone::get_x()
{
    return x;
}

int
Zone::get_y()
{
    return y;
}

std::vector<Block*>::iterator
Zone::get_blocks_begin()
{
    return blocks.begin();
}

std::vector<Block*>::iterator
Zone::get_blocks_end()
{
    return blocks.end();
}

std::vector<Spikes*>::iterator
Zone::get_spikes_begin()
{
    return spikes.begin();
}

std::vector<Spikes*>::iterator
Zone::get_spikes_end()
{
    return spikes.end();
}

void
Zone::set_xy(int x, int y)
{
    this->x = x;
    this->y = y;
}

void
Zone::add_block(Block* block)
{
    this->blocks.push_back(block);
}

void
Zone::add_spikes(Spikes* spikes)
{
    this->spikes.push_back(spikes);
}

bool
Zone::is_xy_same(int x, int y)
{
    return (x == this->x && y == this->y);
}

void
Zone::draw(SDL_Surface *dest)
{
    std::vector<Block*>::iterator i;
    std::vector<Block*>::iterator bend = blocks.end();

    for (i=blocks.begin(); i<bend; ++i) {
        (*i)->apply_surface(dest);
    }

    std::vector<Spikes*>::iterator j;
    std::vector<Spikes*>::iterator send = spikes.end();

    for (j=spikes.begin(); j<send; ++j) {
        (*j)->apply_surface(dest);
    }
}

std::vector<Block*>
Zone::overlap_checks(Rect* r)
{
    std::vector<Block*> overlap_blocks;
    std::vector<Block*>::iterator i;
    std::vector<Block*>::iterator end = blocks.end();

    for (i = blocks.begin(); i < end; ++i) {
        if ((*i)->overlap(r)) {
            overlap_blocks.push_back(*i);
        }
    }

    std::vector<Spikes*>::iterator j;
    std::vector<Spikes*>::iterator send = spikes.end();

    for (j = spikes.begin(); j < send; ++j) {
        if ((*j)->overlap(r)) {
            overlap_blocks.push_back(*j);
        }
    }

    return overlap_blocks;
}
