#pragma once
/*
=================
cPlayer.h
- Header file for class definition - SPECIFICATION
- Header file for the Player class which is a child of cSprite class
=================
*/
#ifndef _CPLAYER_H
#define _CPLAYER_H
#include "cSprite.h"

class cPlayer : public cSprite
{
private:
	SDL_Point playerVelocity;

public:
	cPlayer();
	void update(double deltaTime);		// Player update method
	void setPlayerVelocity(SDL_Point carVel);   // Sets the velocity for the car
	SDL_Point getPlayerVelocity();				 // Gets the car velocity
};
#endif