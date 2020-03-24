#ifndef IOMAP_H
#define IOMAP_H

#include <vector>

#include "map.h"
#include "zone.h"

#define MY_ENCODING "UTF-8"
#define XML_INDENT 1

class Map;
/* to avoid circular dependancy */
typedef struct _xmlNode xmlNode;

class IOMap
{
private:
    enum {
        BUFFER_SIZE = 256
    };

    bool error_msg(const char* fmt, ...); // always returns false
    void int_to_str(int val, char* buffer, int size); // clears buffer and converts
    int get_color(const char* c);
    void load_zone(xmlNode* node, Zone* zone, Map* map);

public:
    IOMap();
    ~IOMap();
    bool load_map(const char* path, Map* map);
    bool save_map(const char* path, Map* map);
};

#endif
