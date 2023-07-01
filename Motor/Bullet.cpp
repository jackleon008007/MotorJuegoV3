#include "Bullet.h"
#include "ResourceManager.h"
#include "GLTexture.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifetime, int _bulletDirection)
{
    this->lifetime = lifetime;
    this->direction = direction;
    this->speed = speed;
    this->position.x = position.x + 15;
    this->position.y = position.y + 15;
    this->bulletDirecton = _bulletDirection;
}

Bullet::~Bullet()
{
}

void Bullet::draw(SpriteBatch& spritebatch)
{
    Color color;
    color.set(255, 255, 255, 255);
    glm::vec4 uv(0.0f,0.0f, 1.0f, 1.0f);
    static GLTexture texture = ResourceManager::getTexture("Textures/Circle.png");
    glm::vec4 posAndSize = glm::vec4(position.x, position.y, 15, 15);
    spritebatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::update()
{

    
    switch (bulletDirecton) {
    case 0: {
        position.x += 1*speed;
        position.y += 0;
        break; }
    case 1: {
        position.x -= 1 * speed;
        position.y += 0;
        break; }
    case 2: {
        position.x += 0;
        position.y += 1 * speed;
        break; }
    case 3: {
        position.x += 0;
        position.y -= 1 * speed;
        break; }
    }

    lifetime--;
    if (lifetime == 0) {
        return true;
    }
    return false;
}

bool Bullet::collideWithAgent(Agent* agent)
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
bool Bullet::collideWithLevel(const vector<string>& levelData)
{
    std::vector<glm::vec2> collideTilePosition;


    checkTilePosition(levelData, collideTilePosition, position.x, position.y);

    checkTilePosition(levelData, collideTilePosition, position.x + AGENT_WIDTH/2, position.y);
    checkTilePosition(levelData, collideTilePosition, position.x, position.y + AGENT_WIDTH/2);
    checkTilePosition(levelData, collideTilePosition, position.x + AGENT_WIDTH/2, position.y + AGENT_WIDTH/2);

    if (collideTilePosition.size() == 0) return false;

    for (size_t i = 0; i < collideTilePosition.size(); i++)
    {
        collideWithTile(collideTilePosition[i]);
    }

    return true;
}

void Bullet::checkTilePosition(const vector<string>& levelData, vector<glm::vec2>& collideTilePosition, float x, float y)
{
    glm::vec2 cornesPos = glm::vec2(floor(x / (float)TILE_WIDTH),
        floor(y / (float)TILE_WIDTH));

    if (cornesPos.x < 0 || cornesPos.x >= levelData[0].size() || cornesPos.y < 0 || cornesPos.y >= levelData.size()) {
        return;
    }
    if (levelData[cornesPos.y][cornesPos.x] != '.') {
        collideTilePosition.push_back(cornesPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
    }
}

void Bullet::collideWithTile(glm::vec2 tilePos)
{
    const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
    const float MIN_DISTANCE = AGENT_RADIUS/3 + TILE_RADIUS;

    glm::vec2 centerPosition = position + glm::vec2(AGENT_RADIUS);
    glm::vec2 distVec = centerPosition - tilePos;
    float xdepth = MIN_DISTANCE - abs(distVec.x);
    float ydepth = MIN_DISTANCE - abs(distVec.y);

    if (xdepth > 0 || ydepth > 0) {
        if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0f)) {
            if (distVec.x < 0) {
                position.x -= xdepth;
            }
            else {
                position.x += xdepth;
            }
        }
        else {
            if (distVec.y < 0) {
                position.y -= ydepth;
            }
            else {
                position.y += ydepth;
            }
        }
    }
}
