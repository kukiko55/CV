#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Resources
{
public:
	//sprites
	Sprite sprBackground;
	Sprite sprPlayer1;
	Sprite sprPlayer2;
	Sprite sprPlayer3;
	Sprite sprPlayer4;
	Sprite sprPlayerProjectile1;
	Sprite sprEnemy1;
	Sprite sprEnemy1Red;

public:
	void Load()
	{
		//player
		tBackground.loadFromFile("Resources/background.png");
		tPlayerProjectile1.loadFromFile("Resources/bullet.png");
		tPlayer1.loadFromFile("Resources/spaceship1.png");
		tPlayer2.loadFromFile("Resources/spaceship2.png");
		tPlayer3.loadFromFile("Resources/spaceship3.png");
		tPlayer4.loadFromFile("Resources/spaceship4.png");

		//enemies
		tEnemy1.loadFromFile("Resources/zozo.png");
		tEnemy1Red.loadFromFile("Resources/zozo1.png");

		//player
		sprBackground.setTexture(tBackground);
		sprPlayerProjectile1.setTexture(tPlayerProjectile1);
		sprPlayer1.setTexture(tPlayer1);
		sprPlayer2.setTexture(tPlayer2);
		sprPlayer3.setTexture(tPlayer3);
		sprPlayer4.setTexture(tPlayer4);

		//enemies
		sprEnemy1.setTexture(tEnemy1);
		sprEnemy1Red.setTexture(tEnemy1Red);
	}

private:
	Texture tBackground, tPlayerProjectile1, tPlayer1, tEnemy1, tEnemy1Red, tPlayer2, tPlayer3, tPlayer4;		//textures
};

struct Ship
{
	enum class Type
	{
		Small = 0,
		Big = 1,
		Bigger = 2,
		Biggest = 3
	};

	Ship(float coordX, float coordY)
	{
		x = coordX;
		y = coordY;
	}
	float LegsStartX() const
	{
		return x + 20;
	}
	float LegsEndX() const
	{
		return x + 60;
	}
	float LegsY() const
	{
		return y + 70;
	}

	float x, y;

	Type type = Type::Small;
};

struct ShipDrawer
{
	ShipDrawer(Resources &res, RenderTarget &target) : resources(res), target(target)
	{
		sprites[Ship::Type::Small] = &res.sprPlayer1;
		sprites[Ship::Type::Big] = &res.sprPlayer2;
		sprites[Ship::Type::Bigger] = &res.sprPlayer3;
		sprites[Ship::Type::Biggest] = &res.sprPlayer4;
	}

	void Draw(const Ship &ship)
	{
		Sprite *sprite = sprites[ship.type];
		sprite->setPosition(ship.x, ship.y);
		target.draw(*sprite);
	}

	Resources &resources;
	RenderTarget &target;
	std::map<Ship::Type, Sprite *> sprites;
};

struct Bullet
{
	Bullet(float x, float y, float dx, float dy, int dmg, Sprite *sprite) : x(x), y(y), dx(dx), dy(dy), dmg(dmg), sprite(sprite) { }

	float x, y, dx, dy;
	int dmg;
	Sprite *sprite;
};

struct EnemyLook
{
	EnemyLook(Sprite &sprBasic, Sprite &sprBasicRed) : sprBasic(&sprBasic), sprBasicRed(&sprBasicRed) { }

	void OnHit() { timeBeingHit = 50; }

	Sprite *sprBasic;
	Sprite *sprBasicRed;

	int timeBeingHit = 0;
};

struct Enemy
{
	Enemy(float x, float y, float dx, float dy, int dmg, int hp, EnemyLook drawing) :
		x(x), y(y), dx(dx), dy(dy), dmg(dmg), hp(hp), drawing(drawing) { }

	void Draw(RenderWindow &app);

	float x, y, dx, dy;
	int dmg, hp;

	EnemyLook drawing;
};