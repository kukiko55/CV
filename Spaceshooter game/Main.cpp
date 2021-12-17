#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <string>
#include <map>
#include <functional>
#include <set>
#include <fstream>

#include "define.h"
#include "objects.h"
#include "utils.h"

using namespace sf;

static bool OnShootButton()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

static void UpdatePlayerPos(Ship &laivas)
{
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && laivas.x>=-40)
		laivas.x -= d;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && laivas.x<=WINDOW_WIDTH-50)
		laivas.x += d;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && laivas.y>=0)
		laivas.y -= d;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && laivas.y<=WINDOW_HEIGHT-30)
		laivas.y += d;
}

static void CreateScore(sf::Font &font, sf::Text &score)
{
	font.loadFromFile("Resources/arial.ttf");
	score.setFont(font);
	score.setFillColor(Color::White);
	score.setOutlineThickness(1);
	score.setOutlineColor(Color::Cyan);
	score.setCharacterSize(70);
	score.setString("0");
	score.setPosition(10, 5);
}

int main()
{
	srand((unsigned int)time(nullptr));
	RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Flying thing go pew pew");
	app.setFramerateLimit(144);

	Resources res;
	res.Load();

	sf::Text txtScore;
	sf::Font font;
	CreateScore(font, txtScore);

	int shootCooldown = 0, dmg = 1, timerEnemies = 0;
	unsigned int score = 0;
	bool pause = false;
	int debug = 0;

	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;
	Ship ship(WINDOW_WIDTH / 2-100, 800);
	ShipDrawer shipDrawer(res, app);

	std::ofstream fd("output.txt");

	int enemiesNumber = 5;

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			debug++;
			if (e.type == Event::Closed) {
				fd<<score<<std::endl;
				app.close();
			}
			if (e.type ==sf::Event::KeyReleased && e.key.code==sf::Keyboard::P || e.type ==sf::Event::KeyReleased && e.key.code==sf::Keyboard::Escape) {
				pause = !pause;
			}
		}
		if (pause)
			continue;

		std::cout<<debug<<std::endl;
		if (shootCooldown>=25)
		{
			if (OnShootButton())
			{
				shootCooldown = 0;
				Bullet kulypka(ship.x+36, ship.y-40, 0, -10, dmg, &res.sprPlayerProjectile1);
				bullets.push_back(kulypka);
			}
		}
		shootCooldown++;

		if (enemies.size()==0)
		{
			CreateEnemies(res, enemiesNumber, enemies);
			enemiesNumber += 2;
		}

		// drawing
		app.draw(res.sprBackground);
		DrawEnemies(app, enemies);
		DrawBullets(app, bullets);
		shipDrawer.Draw(ship);
		txtScore.setString(std::to_string(score));
		app.draw(txtScore);

		// update positions
		UpdateBulletsPos(bullets);
		UpdateEnemiesPos(enemies, timerEnemies);
		UpdatePlayerPos(ship);

		// checks
		CheckIfHit(enemies, bullets, score);

		//player levels
		if (score > 1) {
			ship.type = Ship::Type::Big;
			if (score > 4) {
				ship.type = Ship::Type::Bigger;
				if (score > 8)
					ship.type = Ship::Type::Biggest;
			}
		}
		app.display();
	}
	return 0;
}