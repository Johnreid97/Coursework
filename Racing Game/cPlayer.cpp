/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPlayer.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cPlayer::cPlayer() : cSprite()
{
this->playerVelocity = { static_cast<int> ((0.0), (0.0)) };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPlayer::update(double deltaTime)
{
	
	FPoint direction = { 0.0f, 0.0f };

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->playerVelocity.x = this->playerVelocity.x + direction.X;
	this->playerVelocity.y = this->playerVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->playerVelocity.x * deltaTime;
	currentSpritePos.y += this->playerVelocity.y * deltaTime;

	this->playerVelocity.x *= 0.95;
	this->playerVelocity.y *= 0.95;
	
	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
void cPlayer::setPlayerVelocity(SDL_Point playerVel)
{
	playerVelocity = playerVel;
}
/*
=================================================================
Gets the rocket velocity
=================================================================
*/
SDL_Point cPlayer::getPlayerVelocity()
{

return playerVelocity;

}