#include "enemy.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy() {
    isAlive = false;
    moveDelay = 6;
    frameCount = 0;
}

void Enemy::spawn() {
    int side = rand() % 4;
    
    switch(side) {
        case 0:
            x = rand() % 32;
            y = 0;
            break;
        case 1:
            x = 31;
            y = rand() % 32;
            break;
        case 2:
            x = rand() % 32;
            y = 31;
            break;
        case 3:
            x = 0;
            y = rand() % 32;
            break;
    }
    
    hp = 1;
    isAlive = true;
    frameCount = 0;
}

void Enemy::move(int playerX, int playerY) {
    if (!isAlive) return;
    
    frameCount++;
    if (frameCount < moveDelay) return;
    
    frameCount = 0;
    
    if (x < playerX) x++;
    if (x > playerX) x--;
    if (y < playerY) y++;
    if (y > playerY) y--;
}