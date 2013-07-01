#include "engine.hpp"

void is::cleanUp() {
    delete common;
    delete os;
    delete window;
    delete keyboard;
    delete filesystem;
    delete lua;
}
