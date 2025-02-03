#include "screen.h"
#include "keyboard.h"
#include "bullet.h"
#include "enemy.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <random>
#include <chrono>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Обработчик для корректного завершения программы
void cleanup(int signum) {
    // Восстанавливаем настройки терминала
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
    
    exit(signum);
}

// Функция для настройки неблокирующего чтения с клавиатуры
void setupNonblockingInput() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    setupNonblockingInput();
    signal(SIGINT, cleanup);
    
    Screen screen;
    Keyboard keyboard;
    std::deque<Bullet> bullets;
    std::vector<Enemy> enemies(10);
    
    const size_t MAX_BULLETS = 10;
    
    int playerX = 16;
    int playerY = 16;
    int playerHp = 3;
    
    int lastDirectionX = 0;
    int lastDirectionY = -1;
    
    int frameCount = 0;
    int enemySpawnDelay = 300;
    
    int ammo = 10;
    bool isReloading = false;
    int reloadTimer = 0;
    const int RELOAD_TIME = 180;
    
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    
    while (playerHp > 0) {
        frameCount++;
        
        // Обработка перезарядки
        if (isReloading) {
            reloadTimer++;
            if (reloadTimer >= RELOAD_TIME) {
                ammo = 10;
                isReloading = false;
                reloadTimer = 0;
            }
        }

        int direction = keyboard.getDirection();
        
        // Обработка движения
        if (direction == Keyboard::UP && playerY > 0) {
            playerY = playerY - 1;
            lastDirectionX = 0;
            lastDirectionY = -1;
        }
        if (direction == Keyboard::DOWN && playerY < 31) {
            playerY = playerY + 1;
            lastDirectionX = 0;
            lastDirectionY = 1;
        }
        if (direction == Keyboard::LEFT && playerX > 0) {
            playerX = playerX - 1;
            lastDirectionX = -1;
            lastDirectionY = 0;
        }
        if (direction == Keyboard::RIGHT && playerX < 63) {
            playerX = playerX + 1;
            lastDirectionX = 1;
            lastDirectionY = 0;
        }

        // Обработка стрельбы
        if (direction == Keyboard::SPACE) {
            if (ammo > 0 && !isReloading && bullets.size() < MAX_BULLETS) {
                Bullet newBullet;
                newBullet.fire(playerX, playerY, lastDirectionX, lastDirectionY);
                bullets.push_back(newBullet);
                ammo--;
            } else if (ammo == 0 && !isReloading) {
                isReloading = true;
                reloadTimer = 0;
            }
        }

        // Спавн врагов
        if (frameCount % enemySpawnDelay == 0) {
            auto it = std::find_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return !e.isActive(); });
            if (it != enemies.end()) {
                it->spawn();
            }
        }

        // Обновление пуль и проверка столкновений
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return !b.isActive(); }),
            bullets.end()
        );

        for (auto& bullet : bullets) {
            bullet.move();
            
            // Проверяем попадание пули во врага
            auto enemyIt = std::find_if(enemies.begin(), enemies.end(),
                [&bullet](const Enemy& e) {
                    return e.isActive() && 
                           bullet.getX() == e.getX() && 
                           bullet.getY() == e.getY();
                });
            
            if (enemyIt != enemies.end()) {
                *enemyIt = Enemy();  // Уничтожаем врага
                bullet = Bullet();   // Уничтожаем пулю
            }
        }

        // Обновление врагов
        for (auto& enemy : enemies) {
            if (enemy.isActive()) {
                enemy.move(playerX, playerY);
                
                if (enemy.getX() == playerX && enemy.getY() == playerY) {
                    playerHp--;
                    enemy = Enemy();
                }
            }
        }

        screen.draw(playerX, playerY, bullets, enemies, playerHp, ammo, 
                   isReloading, reloadTimer, RELOAD_TIME);

        // Контроль FPS с помощью std::chrono
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto frameTime = std::chrono::duration_cast<std::chrono::microseconds>
            (currentTime - lastFrameTime).count();
        
        if (frameTime < 16666) { // 60 FPS
            usleep(16666 - frameTime);
        }
        lastFrameTime = std::chrono::high_resolution_clock::now();
    }
    
    screen.showGameOver();
    return 0;
} 