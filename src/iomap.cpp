#include "iomap.h"
#include "map_point.h"
#include "block.h"
#include "spikes.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstdarg>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

IOMap::IOMap()
{

}

IOMap::~IOMap()
{
    xmlCleanupParser();
}

/* just a compact way of printing error msg and returning false */
bool
IOMap::error_msg(const char* fmt, ...)
{
    /* enable printing of formatted string */
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    return false;
}

void
IOMap::int_to_str(int val, char* buffer, int size)
{
    memset(buffer, '\0', size);
    sprintf(buffer, "%d", val);
}

int
IOMap::get_color(const char *c)
{
    if (strcmp(c, "r") == 0) {
        return (rand()%255);
    }

    return atoi(c);
}

/* TODO: fix this function so that it properly loads the map parameter */
bool
IOMap::load_map(const char *path, Map* map)
{
    if (map == NULL) {
        printf("IOMap::load_map() map was NULL\n");
        return false;
    }

    xmlDoc *doc = NULL;
    xmlNode *root = NULL;
    xmlNode *cur = NULL;
    xmlNode *node = NULL;
    xmlNode *node2 = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(path, NULL, 0);

    if (doc == NULL) {
        printf("Unable to load/parse map %s\n", path);
        /* TODO: return null 'equivalent' */
        return false;
    }

    root = xmlDocGetRootElement(doc);

    for (cur = root; cur; cur = cur->next) {
        if (cur->type == XML_ELEMENT_NODE) {
            if (strcmp((char*)cur->name, "map") == 0) {
                for (node = cur->children; node; node = node->next) {
                    if (node->type == XML_ELEMENT_NODE) {
                        if (strcmp((char*)node->name, "zones") == 0) {
                            for (node2 = node->children; node2; node2 = node2->next) {
                                if (node2->type == XML_ELEMENT_NODE) {
                                    if (strcmp((char*)node2->name, "zone") == 0) {
                                        Zone* zone = new Zone();
                                        load_zone(node2, zone, map);
                                        map->zones.push_back(zone);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    xmlFreeDoc(doc);

    //was causing crash in wxWidgets for some reason.
    //xmlCleanupParser();

    return true;
}

void
IOMap::load_zone(xmlNode *root, Zone* zone, Map* map)
{
    if (zone == NULL) {
        printf("zone parameter NULL in IOMap::load_zone(,)\n");
        return;
    }

    xmlNode *node;
    xmlChar *c = NULL;

    int x = 0;
    int y = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    int w = 0;

    c = xmlGetProp(root, (xmlChar*)"x");
    if (c != NULL) {
        x = atoi((char*)c);
    }
    xmlFree(c);

    c = xmlGetProp(root, (xmlChar*)"y");
    if (c != NULL) {
        y = atoi((char*)c);
    }
    xmlFree(c);

    c = xmlGetProp(root, (xmlChar*)"name");
    if (c != NULL) {
        //g_logger->debugf(1, "Loading zone %s", c);
    }
    xmlFree(c);

    zone->set_xy(x, y);

    for (node = root->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            c = NULL;

            if (strcmp((char*)node->name, "spawn") == 0) {
                map->create_spawn();

                /* store which zone contains the spawn */
                map->spawn->set_zone_x(zone->get_x());
                map->spawn->set_zone_y(zone->get_y());

                c = xmlGetProp(node, (xmlChar*)"x");
                if (c != NULL) {
                    map->spawn->set_x(atoi((char*)c));
                }
                xmlFree(c);

                c = xmlGetProp(node, (xmlChar*)"y");
                if (c != NULL) {
                    map->spawn->set_y(atoi((char*)c));
                }
                xmlFree(c);
            }

            if (strcmp((char*)node->name, "goal") == 0) {
                map->create_goal();

                /* store the pos of the zone which contains goal */
                map->goal->set_zone_x(zone->get_x());
                map->goal->set_zone_y(zone->get_y());

                c = xmlGetProp(node, (xmlChar*)"x");
                if (c != NULL) {
                    map->goal->set_x(atoi((char*)c));
                }
                xmlFree(c);

                c = xmlGetProp(node, (xmlChar*)"y");
                if (c != NULL) {
                    map->goal->set_y(atoi((char*)c));
                }
                xmlFree(c);
            }

            if (strcmp((char*)node->name, "rect") == 0) {
                Block *rect = new Block();

                c = xmlGetProp(node, (xmlChar*)"x");
                if (c != NULL) {
                    rect->set_x(atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"y");
                if (c != NULL) {
                    rect->set_y(atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"w");
                if (c != NULL) {
                    w = atoi((char*)c);
                    rect->set_width(w);
                }
                xmlFree(c);

                c = xmlGetProp(node, (xmlChar*)"h");
                if (c != NULL) {
                    rect->set_height(atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"r");
                if (c != NULL) {
                    r = get_color((char*)c);
                    rect->set_r(r);
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"g");
                if (c != NULL) {
                    g = get_color((char*)c);
                    rect->set_g(g);
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"b");
                if (c != NULL) {
                    b = get_color((char*)c);
                    rect->set_b(b);
                }
                xmlFree(c);

                rect->set_color(r, g, b);

                c = xmlGetProp(node, (xmlChar*)"on_impact");
                if (c != NULL) {
                    //obj->on_impact = (e_act)atoi((char*)c);
                }
                xmlFree(c);

                zone->add_block(rect);
            } else if (strcmp((char*)node->name, "line") == 0) {
                /*
                Line *obj = new Line();

                c = xmlGetProp(node, (xmlChar*)"x");
                if (c != NULL) {
                    obj->set_x(atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"y");
                if (c != NULL) {
                    obj->set_y(atoi((char*)c));
                }

                c = xmlGetProp(node, (xmlChar*)"x2");
                if (c != NULL) {
                    obj->o.loc2.x = (atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"y2");
                if (c != NULL) {
                    obj->o.loc2.y = (atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"on_impact");
                if (c != NULL) {
                    obj->on_impact = (e_act)atoi((char*)c);
                }
                xmlFree(c);

                obj->make_texture();
                textures.push_back(obj);
                */
            } else if (strcmp((char*)node->name, "cp") == 0) {
                /*
                Checkpoint *obj = new Checkpoint();

                c = xmlGetProp(node, (xmlChar*)"x");
                if (c != NULL) {
                    obj->set_x(atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"y");
                if (c != NULL) {
                    obj->set_y(atoi((char*)c));
                }
               
                c = xmlGetProp(node, (xmlChar*)"flipped");
                if (c != NULL) {
                    obj->flipped = (bool)atoi((char*)c);

                    if (obj->flipped) {
                        obj->set_rotation(180);
                    }
                }
                xmlFree(c);

                obj->zone_x = x;
                obj->zone_y = y;

                textures.push_back(obj);
                */
            } else if (strcmp((char*)node->name, "spikes") == 0) {
                int x, y, w, h, dir;

                c = xmlGetProp(node, (xmlChar*)"x");
                if (c != NULL) {
                    x = (atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"y");
                if (c != NULL) {
                    y = (atoi((char*)c));
                }
                xmlFree(c);
               
                c = xmlGetProp(node, (xmlChar*)"dir");
                if (c != NULL) {
                    dir = atoi((char*)c);
                }
                xmlFree(c);

                c = xmlGetProp(node, (xmlChar*)"h");
                if (c != NULL) {
                    h = atoi((char*)c);
                }
                xmlFree(c);

                c = xmlGetProp(node, (xmlChar*)"w");
                if (c != NULL) {
                    w = atoi((char*)c);
                }
                xmlFree(c);

                Spikes* spikes = new Spikes(x, y, w, h, dir);
                zone->add_spikes(spikes);
            }
        }
    }
}

bool
IOMap::save_map(const char* path, Map* map)
{
    printf("iomap: in save_map\n");
    int rc;
    char buffer[BUFFER_SIZE];
    xmlTextWriterPtr writer;

    if (map->zones.empty()) {
        return error_msg("iomap: zones.empty()\n");
    }

    writer = xmlNewTextWriterFilename(path, 0);
    if (writer == NULL) {
        return error_msg("iomap: xmlNewTextWriterFilename: error creating the xml writer\n");
    }

    rc = xmlTextWriterSetIndent(writer, XML_INDENT);
    if (rc < 0) {
        return error_msg("iomap: xmlTextWriterSetIndent: error\n");
    }

    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        return error_msg("iomap: xmlTextWriterStartDocument: error\n");
    }

    rc = xmlTextWriterWriteComment(writer,
            (xmlChar*)"This file was created by the map editor which was created by Anders Schanche\n");
    if (rc < 0) {
        return error_msg("iomap: xmlTextWriterWriteComment: error\n");
    }

    /* start looping through the zones and write stuff to the file */

    /* root element*/
    rc = xmlTextWriterStartElement(writer, (xmlChar*)"map");
    if (rc < 0) {
        return error_msg("iomap: xmlTextWriterStartElement: \"map\": error\n");
    }

    rc = xmlTextWriterStartElement(writer, (xmlChar*)"zones");
    if (rc < 0) {
        return error_msg("iomap: xmlTextWriterStartElement: \"zones\": error\n");
    }

    std::vector<Zone*>::iterator current_zone;
    std::vector<Zone*>::iterator end_zone = map->zones.end();

    for (current_zone = map->zones.begin(); current_zone < end_zone; ++current_zone)
    {
        Zone* curr_zone = (*current_zone);

        rc = xmlTextWriterStartElement(writer, (xmlChar*)"zone");
        if (rc < 0) {
            return error_msg("iomap: xmlTextWriterStartElement: \"zone\": error\n");
        }

        int_to_str(curr_zone->get_x(), buffer, BUFFER_SIZE);
        rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"x", (xmlChar*)buffer);
        if (rc < 0) {
            return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_zone->get_x()\": error\n");
        }

        int_to_str(curr_zone->get_y(), buffer, BUFFER_SIZE);
        rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"y", (xmlChar*)buffer);
        if (rc < 0) {
            return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_zone->get_y()\": error\n");
        }

        /* write information about spawn point */
        if (map->is_spawn_zone(curr_zone)) {
            rc = xmlTextWriterStartElement(writer, (xmlChar*)"spawn");
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterStartElement: \"spawn\": error\n");
            }

            int_to_str(map->spawn->get_x(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"x", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"map->spawn->get_x()\": error\n");
            }

            int_to_str(map->spawn->get_y(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"y", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"map->spawn->get_y()\": error\n");
            }

            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterEndElement: \"spawn\": error\n");
            }
        }

        /* write information about spawn point */
        if (map->is_goal_zone(curr_zone)) {
            rc = xmlTextWriterStartElement(writer, (xmlChar*)"goal");
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterStartElement: \"goal\": error\n");
            }

            int_to_str(map->goal->get_x(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"x", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"map->goal->get_x()\": error\n");
            }

            int_to_str(map->goal->get_y(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"y", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"map->goal->get_y()\": error\n");
            }

            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterEndElement: \"goal\": error\n");
            }
        }

        /* iterate over objects in the zone and write to file */
        std::vector<Block*>::iterator current_block;
        std::vector<Block*>::iterator end_block = curr_zone->get_blocks_end();
        
        for (current_block = curr_zone->get_blocks_begin(); current_block < end_block; ++current_block)
        {
            Block* curr_block = (*current_block);

            rc = xmlTextWriterStartElement(writer, (xmlChar*)"rect");
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterStartElement: \"rect\": error\n");
            }

            /* write relevant information for the block */
            int_to_str(curr_block->get_width(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"w", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_block.get_width()\": error\n");
            }

            int_to_str(curr_block->get_height(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"h", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_block.get_height()\": error\n");
            }

            int_to_str(curr_block->get_r(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"r", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterAttribute: \"curr_block.get_r()\": error\n");
            }

            int_to_str(curr_block->get_g(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"g", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterAttribute: \"curr_block.get_g()\": error\n");
            }

            int_to_str(curr_block->get_b(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"b", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterAttribute: \"curr_block.get_b()\": error\n");
            }

            int_to_str(curr_block->get_alpha(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"a", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterAttribute: \"curr_block.get_alpha()\": error\n");
            }

            int_to_str(curr_block->get_x(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"x", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterAttribute: \"curr_block.get_x()\": error\n");
            }

            int_to_str(curr_block->get_y(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"y", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterAttribute: \"curr_block.get_y()\": error\n");
            }

            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterEndElement: \"rect\": error\n");
            }
        }

        /* write spikes */
        std::vector<Spikes*>::iterator current_spikes;
        std::vector<Spikes*>::iterator end_spikes = curr_zone->get_spikes_end();

        for (current_spikes = curr_zone->get_spikes_begin(); current_spikes < end_spikes; ++current_spikes)
        {
            /* write relevant information about spikes */
            Spikes* curr_spikes = (*current_spikes);

            rc = xmlTextWriterStartElement(writer, (xmlChar*)"spikes");
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterStartElement: \"spikes\": error\n");
            }

            int_to_str(curr_spikes->get_dir(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"dir", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_spikes->get_dir\": error\n");
            }

            int_to_str(curr_spikes->get_width(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"w", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_spikes->get_width()\": error\n");
            }

            int_to_str(curr_spikes->get_height(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"h", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_spikes->get_height()\": error\n");
            }

            int_to_str(curr_spikes->get_x(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"x", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_spikes->get_x()\": error\n");
            }

            int_to_str(curr_spikes->get_y(), buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"y", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"curr_spikes->get_y()\": error\n");
            }

            int_to_str(1, buffer, BUFFER_SIZE);
            rc = xmlTextWriterWriteAttribute(writer, (xmlChar*)"on_impact", (xmlChar*)buffer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterWriteAttribute: \"on_impact 1\": error\n");
            }

            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                return error_msg("iomap: xmlTextWriterEndElement: \"spikes\": error\n");
            }
        }

        rc = xmlTextWriterEndElement(writer);
        if (rc < 0) {
            return error_msg("iomap: xmlTextWriterEndElement: \"zone\": error\n");
        }
    }

    /* closes following elements as well as the document: zones, map */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        return error_msg("xmlTextWriterEndDocument: error\n");
    }

    xmlFreeTextWriter(writer);

    return true;
}
