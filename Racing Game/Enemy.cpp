/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cEnemies.h"
#include "cGame.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/

cEnemy::cEnemy() : cSprite()
{

}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cEnemy::update(double deltaTime)
{


	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	cout << "Enemy position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());
	

	
		if (this->getSpritePos().y == (768 - this->getSpritePos().h))
		{
			this->setSpritePos({ 100 * (rand() % 8 + 1), 0 });

		}
	
}
/*
=================================================================
Sets the velocity for the Asteroid
=================================================================
*/
void cEnemy::setEnemyVelocity(SDL_Point AsteroidVel)
{
	enemyVelocity = AsteroidVel;
}
/*
=================================================================
Gets the Asteroid velocity
=================================================================
*/
SDL_Point cEnemy::getEnemyVelocity()
{
	return enemyVelocity;
}
