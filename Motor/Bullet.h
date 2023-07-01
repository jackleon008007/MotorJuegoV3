#pragma once
#include <glm/glm.hpp>
#include "SpriteBatch.h"
#include "Agent.h"
class Bullet
{
private:
	float speed;
	glm::vec2 position;
	glm::vec2 direction;
	int lifetime;
	int bulletDirecton;
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifetime, int _bulletDirection);
	~Bullet();
	void draw(SpriteBatch& spritebatch);
	bool update();
	bool collideWithAgent(Agent* agent);
	bool collideWithLevel(const vector<string>& levelData);
	void checkTilePosition(const vector<string>& levelData, vector<glm::vec2>& collideTilePosition, float x, float y);
	void collideWithTile(glm::vec2 tilePos);
};

