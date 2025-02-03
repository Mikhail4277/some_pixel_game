#include "keyboard.h"
#include <termios.h>
#include <unistd.h>
#include <errno.h>

namespace {
    void setupTerminal() {
        struct termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
    }

    char readChar() {
        char c;
        if (read(STDIN_FILENO, &c, 1) < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return 0;
            }
        }
        return c;
    }
}

int Keyboard::getDirection() {
    static bool isSetup = false;
    if (!isSetup) {
        setupTerminal();
        isSetup = true;
    }

    char key = readChar();
    
    if (key == 0) {
        return NONE;
    }

    if (key == ' ') {
        return SPACE;
    }

    if (key == '\033') {
        char next = readChar();
        if (next == 0) return NONE;
        
        char code = readChar();
        if (code == 0) return NONE;
        
        if (code == 'A') return UP;
        if (code == 'B') return DOWN;
        if (code == 'C') return RIGHT;
        if (code == 'D') return LEFT;
    }

    return NONE;
}