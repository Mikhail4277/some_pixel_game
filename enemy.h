#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
private:
    int x, y;
    int hp;
    bool isAlive;
    int frameCount;
    int moveDelay;

public:
    Enemy();
    void spawn();
    void move(int playerX, int playerY);
    bool isActive() const { return isAlive; }
    int getX() const { return x; }
    int getY() const { return y; }
    void hit() { hp--; if(hp <= 0) isAlive = false; }
    int getHp() const { return hp; }
    void stop() { isAlive = false; }
};
#endif