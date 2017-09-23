#include <cstdlib>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <math.h>

#include <SFML/Graphics.hpp>

#include "robot.h"

#define numParticles 100

class particle
{
public:
	float x;
	float w;
	bool operator<(const particle& p) const {return w < p.w;}
};

using namespace std;

Robot robby; // Robby the robot

bool paused; // Use to pause the simulation

float perceptions[70]; // 1D occupancy grid

particle particles[numParticles]; // Particles NOTE: you can alter this to use STL structures if you prefer

float estimatedPosition; // Final estimate position of the robot

float getNormalRand(float c, float sigma)
{
	float x1,x2,w;
	x1 = (float)((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
	x2 = (float)((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
	while(sqrt(x1*x1+x2*x2) > 1.0)
	{
		x1 = (float)((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
		x2 = (float)((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;                 
	}
	w = sqrt((-2.0 * log(x1*x1+x2*x2)) / (x1*x1+x2*x2));

	return c + (x1 * w) * sigma; 
}

void updateParticles() // Run particle filter
{
	particle part[100];
	//initialize new particles 
	for (int i = 0; i < 100; ++i) {
		particle a;
		a.x = rand() % 700 + 50.0;
		a.w = 1 / (float)numParticles;
		part[i] = a;

	}
	double sum{ 0 };

	double rnd = rand() / RAND_MAX;
	int indx{ 0 };
	for (int a = 0; a < 100; ++a) {

		sum += particles[a].w;
		if (sum > rnd)
		{
			 indx = a;
			break;
		}
	}
	float c = particles[indx].x;

	float result=getNormalRand(c, 2);

	float xt = c + result;







	
}

int main(int argc, char *argv[])
{
	srand(time(NULL)); // Pseudo random number gen - seed it
	paused = false; // Not paused to start woth

	sf::Clock timer;

	// Set up perceptions (load map)
	for(int i = 0;i < 70;i++)
	{
		perceptions[i] = 0.2;
	}
	perceptions[14] = 0.6;
	perceptions[15] = 0.8;
	perceptions[16] = 0.9;
	perceptions[17] = 0.9;
	perceptions[18] = 0.8;
	perceptions[19] = 0.6;

	perceptions[24] = 0.6;
	perceptions[25] = 0.8;
	perceptions[26] = 0.9;
	perceptions[27] = 0.9;
	perceptions[28] = 0.8;
	perceptions[29] = 0.6;

	perceptions[54] = 0.6;
	perceptions[55] = 0.8;
	perceptions[56] = 0.9;
	perceptions[57] = 0.9;
	perceptions[58] = 0.8;
	perceptions[59] = 0.6;

	// Init particles
	for(int i = 0;i < numParticles; i++)
	{
		particle p;
		p.x = rand() % 700 + 50.0;
		p.w = 1.0 / (float) numParticles;
		particles[i] = p;
	}

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "1D MCL Program");

	// Create the wall
	sf::RectangleShape wall;
	wall.setPosition(50,50);
	wall.setSize(sf::Vector2f(700,100));
	wall.setFillColor(sf::Color(153,103,103)); // Pale red 

	// Create the doors
	sf::RectangleShape door1;
	door1.setPosition(200,75);
	door1.setSize(sf::Vector2f(40,75));
	door1.setFillColor(sf::Color(51,51,51)); // Grey

	sf::RectangleShape door2;
	door2.setPosition(300,75);
	door2.setSize(sf::Vector2f(40,75));
	door2.setFillColor(sf::Color(51,51,51)); // Grey

	sf::RectangleShape door3;
	door3.setPosition(600,75);
	door3.setSize(sf::Vector2f(40,75));
	door3.setFillColor(sf::Color(51,51,51)); // Grey

	// Create axises
	sf::Vertex axises[4];
	// X
	axises[0] = sf::Vertex(sf::Vector2f(40,550),sf::Color::Black);
	axises[1] = sf::Vertex(sf::Vector2f(760,550),sf::Color::Black);
	// Y
	axises[2] = sf::Vertex(sf::Vector2f(45,555),sf::Color::Black);
	axises[3] = sf::Vertex(sf::Vector2f(45,250),sf::Color::Black);

	timer.restart();

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			// p or esc pressed
			if (event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Escape)
					window.close();
				if(event.key.code == sf::Keyboard::P)
					paused = !paused;
			}
		}

		// Update the robot
		if(timer.getElapsedTime().asSeconds() > 0.5f)
		{
			if(!paused)
			{
				robby.move();
				updateParticles();
			}
			timer.restart();
		}

		// clear the window with white
		window.clear(sf::Color::White);

		// draw wall
		window.draw(wall);

		// draw doors
		window.draw(door1);
		window.draw(door2);
		window.draw(door3);

		// Draw perception model
		for(int i = 0;i < 70;i++)
		{
			sf::RectangleShape rect;
			rect.setPosition(50 + i * 10, 160);
			rect.setSize(sf::Vector2f(10,10));
			rect.setFillColor(sf::Color(perceptions[i]*255.f, perceptions[i]*255.f, perceptions[i]*255.f));
			window.draw(rect);
		}

		// Draw axises
		window.draw(axises,4,sf::PrimitiveType::Lines);

		// Draw Particles
		sf::Color particleColour(26,230,26);
		for(int i = 0;i < numParticles;i++)
		{
			sf::VertexArray line(sf::PrimitiveType::Lines, 2);
			line[0] = sf::Vertex(sf::Vector2f(particles[i].x, 550.0f),particleColour);
			line[1] = sf::Vertex(sf::Vector2f(particles[i].x, 550.0f - particles[i].w * (300.0 * numParticles / 10.0)),particleColour);
			window.draw(line);
		}

		// Draw estimated position
		sf::VertexArray line(sf::PrimitiveType::Lines, 2);
		line[0] = sf::Vertex(sf::Vector2f(estimatedPosition, 550.0f),sf::Color::Magenta);
		line[1] = sf::Vertex(sf::Vector2f(estimatedPosition, 250.0f),sf::Color::Magenta);
		window.draw(line);

		// Draw Robby
		window.draw(robby);

		// End the current frame, swap draw buffers
		window.display();
	}

	return EXIT_SUCCESS;
}
