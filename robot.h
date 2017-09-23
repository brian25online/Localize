#ifndef ROBOT_H
#define ROBOT_H

#include <math.h>
#include <stdlib.h>

#include <SFML/Graphics.hpp>

using namespace std;


class Robot : public sf::Drawable {

protected:
	float position; // Position of the robot in 1D space
	float deltaPosition;
	float pi;
	sf::RectangleShape top;
	sf::RectangleShape body;
	sf::CircleShape wheel;
	sf::CircleShape hub;
public:
	Robot();
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void move();
	float getPosition(){return position + (float)(rand() % 50) / 25.0;} // Return incorrect position
	float sonarReading(); // Simulated sonar reading
};

#endif
