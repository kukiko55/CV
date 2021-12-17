#include "define.h"
#include "objects.h"
#include "utils.h"
#include <iostream>

void DrawEnemies(RenderWindow &app, std::vector<Enemy> &enemies)
{
	for (Enemy &enemy : enemies) {
		enemy.Draw(app);
	}
}

void CheckIfHit(std::vector<Enemy> &enemies, std::vector<Bullet> &bullets, unsigned int &score)
{
	for (auto it = enemies.begin(); it!=enemies.end(); it++)
	{
		for (auto h = bullets.begin(); h!=bullets.end(); )
		{
			if (h->y+40>=it->y && h->y-40<=it->y && h->x+40>=it->x && h->x-100<=it->x)
			{
				it->hp -= h->dmg;
				h = bullets.erase(h);
				it->drawing.OnHit();
				if (it->hp <= 0)
					++score;
			}
			else
				h++;
		}
	}
}

void Enemy::Draw(RenderWindow &app)
{
	if (drawing.timeBeingHit<=0)
	{
		drawing.sprBasic->setPosition(x, y);
		app.draw(*drawing.sprBasic);
	}
	else
	{
		--drawing.timeBeingHit;
		drawing.sprBasicRed->setPosition(x, y);
		app.draw(*drawing.sprBasicRed);
	}
}

void CreateEnemies(Resources &resources, const int enemiesCount, std::vector<Enemy> &enemies)
{
	for (int i = 0; i< enemiesCount; ++i)
	{
		EnemyLook el(resources.sprEnemy1, resources.sprEnemy1Red);
		Enemy enemy((float)WINDOW_WIDTH-(rand()%WINDOW_WIDTH), ((float)WINDOW_HEIGHT/3)-(rand()%(WINDOW_HEIGHT/3)), 1, 1, 1, 3, el);
		enemies.push_back(enemy);
	}
}

void UpdateEnemiesPos(std::vector<Enemy> &enemies, int &timer)
{
	++timer;
	if (timer==movementscale)
		timer = 0;

	auto itR = std::remove_if(
		enemies.begin(),
		enemies.end(),
		[](const Enemy &k) { return k.hp<=0; }
	);

	for (auto it = enemies.begin(); it != itR; ++it) {
		if (timer % movementscale <= movementscale/4 && timer % movementscale>=0 && it->y<=WINDOW_HEIGHT-80)
			it->y += rand()%4;
		else
			if (timer % movementscale <= movementscale/2 && timer % movementscale>= movementscale/4 && it->x<=WINDOW_WIDTH-80)
				it->x += rand()%4;
			else if (timer % movementscale <=movementscale - movementscale/4 && timer % movementscale>=movementscale/2 && it->y>=10)
				it->y -= rand()%4;
			else if (timer % movementscale <=movementscale && timer % movementscale>=movementscale - movementscale/4 && it->x>=10)
				it->x -= rand()%4;
	}
	enemies.erase(itR, enemies.end());
}

void DrawBullets(RenderWindow &app, const std::vector<Bullet> &bullets)
{
	for (const Bullet &bullet : bullets) {
		bullet.sprite->setPosition(bullet.x, bullet.y);
		app.draw(*bullet.sprite);
	}
}

bool tikrinimas(Bullet it)
{
	if (it.y<-80)
		return true;
	else
		return false;
}

void UpdateBulletsPos(std::vector<Bullet> &bullets)
{
	//	auto itR = std::remove_if(bullets.begin(), bullets.end(), [](const Bullet &c) { return c.y<-80; });
	auto itR = std::remove_if(bullets.begin(), bullets.end(), tikrinimas);
	for (auto it = bullets.begin(); it != itR; ++it) {
		it->y += it->dy;
		it->x += it->dx;
	}

	bullets.erase(itR, bullets.end());
}