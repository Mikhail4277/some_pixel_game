#ifndef BULLET_H
#define BULLET_H

class Bullet {
private:
    int x, y;
    int directionX;
    int directionY;
    bool isAlive;
    int speed;
    int frameCount;
    int moveDelay;

public:
    Bullet();
    void fire(int startX, int startY, int dirX, int dirY);
    void move();
    bool isActive() const { return isAlive; }
    int getX() const { return x; }
    int getY() const { return y; }
    void stop() { isAlive = false; }
};

#endif