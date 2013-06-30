#include "engine.hpp"

void cleanUp() {
    delete common;
    delete os;
    delete window;
    delete keyboard;
}
