// text.hpp: Text renderable

#ifndef IS_TEXT_H_
#define IS_TEXT_H_

namespace is {

class Text {
public:
                    Text();
                    ~Text();
    void            remove();
    // String identifier
    std::string     type();
    void            tick( float dt );
    void            draw();
};

};

#endif
