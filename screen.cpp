#include "screen.h"
#include "bullet.h"
#include <iostream>
#include <vector>
#include <string>
#include <deque>

Screen::Screen() {
    clear();
    std::cout << "\033[?25l";
    std::cout << "\033[2J";
}

void Screen::clear() {
    std::cout << "\033[2J\033[H";
}

void Screen::draw(int playerX, int playerY, 
                 const std::deque<Bullet>& bullets,
                 const std::vector<Enemy>& enemies,
                 int playerHp, int ammo, bool isReloading,
                 int reloadTimer, int reloadTime) {
    clear();
    
    std::string field[32][64];
    for(int y = 0; y < 32; y++) {
        for(int x = 0; x < 64; x++) {
            if(x == 0 || x == 63 || y == 0 || y == 31) {
                field[y][x] = "#";
            } else {
                field[y][x] = " ";
            }
        }
    }
    
    for(const auto& bullet : bullets) {
        if(bullet.isActive()) {
            field[bullet.getY()][bullet.getX()] = "\033[31m*\033[0m";
        }
    }
    
    for(const auto& enemy : enemies) {
        if(enemy.isActive()) {
            field[enemy.getY()][enemy.getX()] = "\033[31mE\033[0m";
        }
    }
    
    field[playerY][playerX] = "\033[32m@\033[0m";
    
    for(int y = 0; y < 32; y++) {
        for(int x = 0; x < 64; x++) {
            std::cout << field[y][x];
        }
        std::cout << "\n";
    }
    
    std::cout << "HP: " << playerHp << " Ammo: ";
    
    std::cout << " ︻┳═一 ";
    for(int i = 0; i < ammo; i++) {
        std::cout << "•";
    }
    for(int i = ammo; i < 10; i++) {
        std::cout << "○";
    }
    
    if(isReloading) {
        std::cout << " RELOADING... " << (reloadTimer * 100 / reloadTime) << "%";
    }
    std::cout << "\n";
}

void Screen::showGameOver() {
    clear();
    std::cout << "\033[31mGAME OVER\033[0m\n";
}

Screen::~Screen() {
    std::cout << "\033[?25h";
}