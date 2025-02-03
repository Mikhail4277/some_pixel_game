#ifndef SCREEN_H
#define SCREEN_H
#include <vector>
#include <deque>
#include "bullet.h"
#include "enemy.h"

class Screen {
private:
    static const int WIDTH = 32;
    static const int HEIGHT = 32;
    char screen[HEIGHT][WIDTH];

public:
    Screen();
    ~Screen();
    void clear();
    void draw(int playerX, int playerY, const std::deque<Bullet>& bullets, 
             const std::vector<Enemy>& enemies, int playerHp, int ammo, 
             bool isReloading, int reloadTimer, int reloadTime);
    void showGameOver();
};
#endif