#include "Robot.h"

Robot::Robot()
{
	position = 100.0;
	deltaPosition = 0.0;
	pi = 3.1415; // Approx value for pi
	float radius = 12.0;
	
	top.setSize(sf::Vector2f(64,5));
	top.setOrigin(32,2.5f);
	top.setFillColor(sf::Color::Black);

	body.setSize(sf::Vector2f(60,26));
	body.setOrigin(30,13);
	body.setFillColor(sf::Color::Red);

	wheel.setRadius(radius);
	wheel.setOrigin(radius, radius);
	wheel.setFillColor(sf::Color::Black);

	hub.setRadius(radius*0.45f);
	hub.setOrigin(radius*0.45f, radius*0.45f);
	hub.setFillColor(sf::Color::Yellow);

	float x = position;
	float y = 138.0;

	top.setPosition(x,y-23);
	body.setPosition(x,y-8.0);
	wheel.setPosition(x,y);
	hub.setPosition(x,y);
}

void Robot::move()
{
	float delta = (float)(rand() % 50 - 25) / 20.0;
	deltaPosition = delta + 10.0;
	position += deltaPosition;

	float x = position;
	float y = 138.0;

	top.setPosition(x,y-23);
	body.setPosition(x,y-8.0);
	wheel.setPosition(x,y);
	hub.setPosition(x,y);
}

float Robot::sonarReading()
{
	if(position >  200 && position < 240) return 1.0; // First door
	if(position >  300 && position < 340) return 1.0; // Second door
	if(position >  600 && position < 640) return 1.0; // Third door
	return 0.0;
}


void Robot::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(top);
	target.draw(body);
	target.draw(wheel);
	target.draw(hub);
}


