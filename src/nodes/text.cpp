#include "text.hpp"

is::Text::Text() {

}

is::Text::~Text() {

}

void is::Text::remove() {
    delete (is::Text*)this;
}

std::string is::Text::type() {
    return "text";
}

void is::Text::tick( float dt ) {

}

void is::Text::draw() {

}
