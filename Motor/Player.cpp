#include "Player.h"
#include <SDL/SDL.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, InputManager* inputManager)
{
	this->path = "Textures/circle.png";
	this->speed = speed;
	this->position = position;
	this->inputManager = inputManager;
	color.set(185, 0, 0, 255);
}

void Player::update(const vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	if (inputManager->isKeyPressed(SDLK_w)) {
		position.y += speed;
	}

	if (inputManager->isKeyPressed(SDLK_s)) {
		position.y -= speed;
	}

	if (inputManager->isKeyPressed(SDLK_a)) {
		position.x -= speed;
	}

	if (inputManager->isKeyPressed(SDLK_d)) {
		position.x += speed;
	}
	collideWithLevel(levelData);



}

bool Player::collideWithAgent(Agent* agent)
{
	glm::vec2 centerPosA = position + glm::vec2(AGENT_WIDTH / 2);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_WIDTH / 2);
	glm::vec2 dist = centerPosA - centerPosB;
	const float MIN_DISTANCE = AGENT_RADIUS * 2;
	float distance = glm::length(dist);
	float collision = MIN_DISTANCE - distance;
	if (collision > 0) {
		glm::vec2 collisionVec = glm::normalize(dist) * collision;
		position += collisionVec / 2.0f;
		agent->setPosition(agent->getPosition() + (collisionVec / 2.0f));
		return true;
	}
	return false;
}
