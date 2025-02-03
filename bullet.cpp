#include "bullet.h"

Bullet::Bullet() {
    isAlive = false;
    speed = 1;
    frameCount = 0;
    moveDelay = 2;
}

void Bullet::fire(int startX, int startY, int dirX, int dirY) {
    x = startX;
    y = startY;
    directionX = dirX;
    directionY = dirY;
    isAlive = true;
    frameCount = 0;
}

void Bullet::move() {
    if (!isAlive) {
        return;
    }

    frameCount++;

    if (frameCount >= moveDelay) {
        x = x + (directionX * speed);
        y = y + (directionY * speed);
        frameCount = 0;

        if (x < 0 || x >= 32 || y < 0 || y >= 32) {
            isAlive = false;
        }
    }
}