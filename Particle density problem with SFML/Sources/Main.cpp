#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cmath>
#include <fstream>
#include <functional>

#include <SFML/Graphics.hpp>

using namespace std;

const float scale = 20.f; //scale for drawing

struct Particle {
	double r, x, y;
	Particle(double r1, double x1, double y1) {
		r = r1;
		x = x1;
		y = y1;
	}
	Particle() {
		r = 0;
		x = 0;
		y = 0;
	}
};

int n;
double l;
vector<double> radii;
vector<vector<Particle>> result;

//Drawing with sfml
void Draw() {
	sf::RenderWindow app(sf::VideoMode(500, 1000), "Particles");
	app.setFramerateLimit(60);

	while (app.isOpen())
	{
		app.display();
		sf::Event e;
		while (app.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				app.close();
		}
		for (size_t i = 0; i<result.size(); i++)
		{
			for (size_t j = 0; j<result[i].size(); j++) {
				sf::CircleShape shape((float)result[i][j].r*scale);
				shape.setPosition((float)(result[i][j].x - result[i][j].r)*scale, (float)(50-result[i][j].y - result[i][j].r)*scale);
				shape.setFillColor(sf::Color(sf::Uint8((result[i][j].y / 50.f) * 255), sf::Uint8(result[i][j].x / 30.f * 255), sf::Uint8(result[i][j].r * 255))); //Fun color shuffle
				shape.setPointCount(240);
				app.draw(shape);
			}
		}
	}
}

//Read, sort, and copy creation
void Input() {
	ifstream fd("input.txt");
	if (fd.fail())
	{
		cout << "File not found" << endl;
		system("pause");
		exit(-1);
	}
	fd>>n>>l;
	radii.resize(n);
	result.resize(n);
	for (int i = 0; i<n; i++) {
		double t;
		fd>>t;
		radii[i] = t;
	}
	fd.close();
	sort(radii.begin(), radii.end(), greater<double>());
}

void Output(double height, int sum) {
	ofstream fs("output.txt");
	fs<<"Height: "<<height<<", Sum: "<<sum<<endl;
	fs << "x\ty\tr" << endl;
	fs << fixed << setprecision(3);
	for (auto &line : result) {
		for (auto &element : line) {
			fs<< element.x << "\t" << element.y << "\t" << element.r << endl;
		}
	}
	fs.close();
}

void LeftCorner(size_t i) {
	vector<Particle> &eile = result[0];
	double maxradius = (eile[i].r*sqrt(2.)-eile[i].r)/2;
	auto t = lower_bound(radii.begin(), radii.end(), maxradius, greater<double>());
	if (t!=radii.end()) {
		double r = *t;
		double x = (eile[i].r*sqrt(2.)-result[0][i].r)/2;
		double y = (eile[i].r*sqrt(2.)-result[0][i].r)/2;
		Particle temp(r, x, y);
		result[0].insert(eile.begin()+i, temp);
		radii.erase(t);
	}
}

void InBetweenParticles(int i) {
	double k1 = 1/result[0][result[0].size()-2].r;	//coefficients for descarted theorem
	double k2 = 1/result[0][result[0].size()-1].r;
	double saknis = 2*sqrt(k1*k2);
	double maxradiuskof = k1+k2+saknis;	//descartes theorem
	double maxradius = 1/maxradiuskof;
	auto t = lower_bound(radii.begin(), radii.end(), maxradius, greater<double>());
	if (t!=radii.end()) { //Can we put an additional particle touching the bottom according to "descartes" theorem
		double r = *t;
		double y = *t;
		double x = result[0][result[0].size()-2].x + sqrt(pow((result[0][result[0].size()-2].r+*t), 2)-pow((result[0][result[0].size()-2].r-*t), 2));
		Particle temp(r, x, y);
		result[0].insert(result[0].begin()+i, temp);
		radii.erase(t);
	}
}

void MakingBase() {
	size_t i = 0;
	double left = 0;
	Particle temp(radii[0], radii[0], radii[0]);
	result[0].push_back(temp);
	LeftCorner(i);
	left += result[0][i].r;
	radii.erase(radii.begin());		//Delete what we put down
	while (left<=l && i<radii.size()) {
		if (left+sqrt(pow((result[0][result[0].size()-1].r+radii[i]), 2)-pow((result[0][result[0].size()-1].r-radii[i]), 2))+radii[i]*2<=l) {	//Can we put a particle on the right
			double r = radii[i];
			double x = result[0][result[0].size()-1].x + sqrt(pow((result[0][result[0].size()-1].r+radii[i]), 2)-pow((result[0][result[0].size()-1].r-radii[i]), 2));
			double y = radii[i];
			Particle temp1(r, x, y);
			result[0].push_back(temp1);
			left += sqrt(pow((result[0][result[0].size()-1].r+radii[i]), 2)-pow((result[0][result[0].size()-1].r-radii[i]), 2));
			InBetweenParticles(i); //Can we put an additional particle according to "descartes" theorem
			radii.erase(radii.begin()+i);
			continue;
		}
		i++;
	}
}

void AdditionalParticleCheck(int line, int curr, int curr2, double &Cx, double &Cy, size_t i) {	//Searching for 3rd point of the triangle according to simple math proof
	double xtemp1 = result[line-1][curr].x; // 1 - A
	double ytemp1 = result[line-1][curr].y;
	double rtemp1 = result[line-1][curr].r;
	double xtemp2 = result[line][curr2].x; // 2 - B
	double ytemp2 = result[line][curr2].y;
	double rtemp2 = result[line][curr2].r;
	double rtemp3 = radii[i]; // 3 - C
	double AB = sqrt(pow(xtemp1-xtemp2, 2)+pow(ytemp1-ytemp2, 2));
	double AC = rtemp1 + rtemp3; // l2
	double BC = rtemp2 + rtemp3; // l3
	double cosalpha = (xtemp2-xtemp1)/AB;
	double sinalpha = (ytemp2-ytemp1)/AB;
	double cosbeta = (pow(AC, 2)+pow(AB, 2)-pow(BC, 2))/(2*AC*AB);
	double sinbeta = sqrt(1-pow(cosbeta, 2));
	double cosgamma = cosalpha*cosbeta - sinalpha*sinbeta;
	double singamma = sinalpha*cosbeta + cosalpha*sinbeta;
	Cx = xtemp1 + AC * cosgamma;
	Cy = ytemp1 + AC * singamma;
}

void NextLevels(int line) {
	double right = l;
	int curr = (int)result[line-1].size()-1;
	double hypotenuse = result[line-1][curr].r + radii[0];
	double x1 = l-radii[0];
	double y1 = sqrt(pow(hypotenuse, 2)-pow(abs(x1-result[line-1][curr].x), 2)) + result[line-1][curr].y; //Search for most right particle on the next level
	if (curr > 0 && result[line-1][curr-1].r+radii[0] > sqrt(pow((result[line-1][curr-1].x-x1), 2)+pow((result[line-1][curr-1].y-y1), 2))) //Check which particle to glue next to
	{
		curr -= 1;
		hypotenuse = result[line-1][curr].r + radii[0];
		y1 = sqrt(pow(hypotenuse, 2)-pow(abs(x1-result[line-1][curr].x), 2))+result[line-1][curr].y;
	}
	right = x1-radii[0];
	Particle temp(radii[0], x1, y1);
	radii.erase(radii.begin());
	result[line].push_back(temp);
	size_t i = 0;
	while (right>=0 && i<radii.size()) {
		int curr2 = (int)result[line].size()-1;
		double Cx, Cy;
		AdditionalParticleCheck(line, curr, curr2, Cx, Cy, i);
		if (curr>0 && result[line-1][curr-1].r+radii[i] > sqrt(pow((result[line-1][curr-1].x-Cx), 2)+pow((result[line-1][curr-1].y-Cy), 2))) {
			curr--;
			AdditionalParticleCheck(line, curr, curr2, Cx, Cy, i);
		}
		if (Cx-radii[i]>=0)
			right = Cx-radii[i];
		else {
			i++;
			continue;
		}
		Particle temp1(radii[i], Cx, Cy);
		radii.erase(radii.begin());
		result[line].push_back(temp1);
	}
	reverse(result[line].begin(), result[line].end());	//Reverse for simpler code
}

int main() {
	Input();
	MakingBase();
	int line = 1;
	while (radii.size()>0) {
		NextLevels(line);
		line++;
	}
	int sum = 0;
	double height = 0;
	for (size_t i = 0; i<result.size(); i++) {
		for (size_t j = 0; j<(int)result[i].size(); j++) {
			height = max(height, result[i][j].y+result[i][j].r);
		}
		sum += result[i].size();
	}

	Output(height, sum);

	cout<<"Height: "<<height<<", Sum: "<<sum<<endl;

	Draw();

	return 0;
}