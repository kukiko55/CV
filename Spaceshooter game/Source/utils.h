#pragma once
#include "objects.h"
#include "define.h"

using namespace sf;

void DrawEnemies(RenderWindow &app, std::vector<Enemy> &enemies);
void CheckIfHit(std::vector<Enemy> &enemies, std::vector<Bullet> &bullets, unsigned int &score);
void CreateEnemies(Resources &resources, const int enemiesCount, std::vector<Enemy> &enemies);
void UpdateEnemiesPos(std::vector<Enemy> &enemies, int &timer);
void DrawBullets(RenderWindow &app, const std::vector<Bullet> &bullets);
bool tikrinimas(Bullet it);
void UpdateBulletsPos(std::vector<Bullet> &bullets);