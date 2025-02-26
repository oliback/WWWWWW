######################################################################
# Automatically generated by qmake (2.01a) Tue Jun 4 00:55:44 2013
######################################################################

TEMPLATE = app
TARGET = hello
INCLUDEPATH += usr/include
LIBS += -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -ggdb `xml2-config --cflags --libs` `wx-config --libs` `wx-config --cxxflags`

# Input
HEADERS += button.h \
           game.h \
           helpful_functions.h \
           iomap.h \
           map.h \
           map_editor.h \
           map_point.h \
           mini_map.h \
           player.h \
           rect_object.h \
           sdl_functions.h \
           spikes.h \
           timer.h \
           widgets.h \
           zone.h
SOURCES += button.cpp \
           game.cpp \
           helpful_functions.cpp \
           iomap.cpp \
           main.cpp \
           map.cpp \
           map_editor.cpp \
           map_point.cpp \
           mini_map.cpp \
           player.cpp \
           rect_object.cpp \
           sdl_functions.cpp \
           spikes.cpp \
           timer.cpp \
           widgets.cpp \
           zone.cpp
