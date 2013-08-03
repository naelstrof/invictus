#include "engine.hpp"

void is::cleanUp() {
    delete states;
    delete common;
    delete os;
    delete textures;
    delete window;
    delete keyboard;
    delete mouse;
    delete filesystem;
    delete lua;
    delete shaders;
    delete glyphs;
    delete fonts;
    delete models;
    delete world;
    delete gui;
    delete render;
}
