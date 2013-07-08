#include "engine.hpp"

void is::cleanUp() {
    delete common;
    delete os;
    delete window;
    delete keyboard;
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
