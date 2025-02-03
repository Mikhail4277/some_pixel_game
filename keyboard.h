#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard {
public:
    enum Keys {
        NONE = 0,
        UP = 1,
        RIGHT = 2,
        DOWN = 3,
        LEFT = 4,
        SPACE = 5,
        RELOAD = 6
    };

    int getDirection();
};

#endif