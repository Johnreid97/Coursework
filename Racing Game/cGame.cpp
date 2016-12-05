/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();

/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{
	
}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}

void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);

	score = 0;

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	theAreaClicked = { 0, 0 };
	
	// Store the textures
	textureName = { "Enemy1", "thePlayer","theBackground" };
	texturesToUse = { "Images\\aieRagqeT.png", "Images\\9TzxXnaxc.png", "Images\\Toon Road Texture.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Store the button textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images//button_exit.png", "Images//button_instructions.png", "Images//button_load.png", "Images//button_menu.png", "Images//button_play.png", "Images//button_save.png", "Images//button_settings.png" };
	btnPos = { { 400, 375 },{ 400, 300 },{ 400, 300 },{ 500, 500 },{ 400, 300 },{ 740, 500 },{ 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;

	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = { "Racer", "Score: ", "Game Over!", "See you again soon!", "Use Arrow Keys to Move!"};
	

	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GamOvertxt", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 255, 0 ,255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GoodbyeTxt", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 0, 255, 0 ,255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 0, 255, 0 ,255 }, { 0, 0, 0, 0 }));
	
	// Load game sounds
	soundList = { "theme", "carNoise", "crash" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio//277363__nyan-cat__8bit-race-music.MP3", "Audio//339700__ffkoenigsegg20012017__car-about-to-pass-by-aggressive.wav", "Audio//332056__qubodup__fast-collision.FLAC" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}
	
	theSoundMgr->getSnd("theme")->play(-1);
	

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	
	
	// Create vector array of textures
		for (int cars = 0; cars < 8; cars++)
		{
			
				theEnemies.push_back(new cEnemy);
				theEnemies[cars]->setSpritePos({ 100 * (rand() % 8 + 1), 0 * (rand() % 8 + 1) });
				theEnemies[cars]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
				int randEnemies = rand() % 4;
				theEnemies[cars]->setTexture(theTextureMgr->getTexture("Enemy1"));
				theEnemies[cars]->setSpriteDimensions(theTextureMgr->getTexture("Enemy1")->getTWidth(), theTextureMgr->getTexture("Enemy1")->getTHeight());
				theEnemies[cars]->setActive(true);
				
			
		}
	
	//Create a vector array of textures for the player
		
			for (int car = 0; car < 1; car++)
			{
				
					player.push_back(new cPlayer);					
					player[car]->setSpritePos({ 500, 640 });
					player[car]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
					int randPlayer = rand() % 4;
					player[car]->setTexture(theTextureMgr->getTexture("thePlayer"));
					player[car]->setSpriteDimensions(theTextureMgr->getTexture("thePlayer")->getTWidth(), theTextureMgr->getTexture("thePlayer")->getTHeight());
					player[car]->setActive(true);
					

			}
		
	SDL_RenderPresent(theRenderer);
	
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	 bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
		
	}
}
void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		score = 0;
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("Instructions");
		pos = { 300, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
		

	}
	break;
	case PLAYING:
	{
		if (theGameState == PLAYING)
		{
			spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
			
			// Render each Enemy in the vector array
			for (int draw = 0; draw < theEnemies.size(); draw++)
			{
				theEnemies[draw]->render(theRenderer, &theEnemies[draw]->getSpriteDimensions(), &theEnemies[draw]->getSpritePos(), theEnemies[draw]->getSpriteRotAngle(), &theEnemies[draw]->getSpriteCentre(), theEnemies[draw]->getSpriteScale());
			}

			// Render the Title
			cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
			SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			FPoint scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);


			//Render the Score
			theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, sScore.c_str(), SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
			tempTextTexture = theTextureMgr->getTexture("Score");
			pos = { 600, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			theButtonMgr->getBtn("exit_btn")->setSpritePos({ 10, 700 });
			theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

			// Render the player

			for (int draw = 0; draw < player.size(); draw++)
			{
				player[draw]->render(theRenderer, &player[draw]->getSpriteDimensions(), &player[draw]->getSpritePos(), player[draw]->getSpriteRotAngle(), &player[draw]->getSpriteCentre(), player[draw]->getSpriteScale());
			}
		}
	}
	break;
	case END:
	{
		
		//Render the Title
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Render Text
		tempTextTexture = theTextureMgr->getTexture("GamOvertxt");
		pos = { 300, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

			tempTextTexture = theTextureMgr->getTexture("GoodbyeTxt");
		pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, sScore.c_str(), SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 300, 300, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Render the button
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
		
		
	}
	break;
	case QUIT:
	{
		 loop = false;
	}
	break;
	default:
		break;
	}
	
		SDL_RenderPresent(theRenderer);
	
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{
	
}

void cGame::update(double deltaTime)
{
	// Update the visibility and position of each enemy car if in the game's main scene
	if (theGameState == PLAYING)
	{
		
		vector<cEnemy*>::iterator enemyIterator = theEnemies.begin();

		while (enemyIterator != theEnemies.end())
		{
			if ((*enemyIterator)->isActive() == false)
			{
				enemyIterator = theEnemies.erase(enemyIterator);
			}
			else
			{
				(*enemyIterator)->update(deltaTime);
				++enemyIterator;

			}
		}
	}
	
	// Update the visibility and position of the Player
		vector<cPlayer*>::iterator playerIterator = player.begin();
		while (playerIterator != player.end())
		{
			if ((*playerIterator)->isActive() == false)
			{
				playerIterator = player.erase(playerIterator);
			}
			else
			{
				(*playerIterator)->update(deltaTime);
				++playerIterator;
			}
		
		
			// Check Button clicked and change state
			if (theGameState == END || theGameState == MENU)
			{
				theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
				
			}
			else
			{
				theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, END, theAreaClicked);
			}
			theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
			theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);

			//resets the position of the player and the enemy cars
			if ( theGameState == MENU || theGameState == END)
			{
				for (vector<cEnemy*>::iterator enemyIterartor = theEnemies.begin(); enemyIterartor != theEnemies.end(); ++enemyIterartor)
				{
					((*enemyIterartor)->setSpritePos({ 100 * (rand() % 8 + 1), 50 * (rand() % 8 + 1) }));

				}

				for (vector<cPlayer*>::iterator playerIterartor = player.begin(); playerIterartor != player.end(); ++playerIterartor)
				{
					((*playerIterartor)->setSpritePos({500,640 }));
				}


			}
			
			//moves the enemy cars if on the main game scene
			if (theGameState == PLAYING)
			{
				for (vector<cEnemy*>::iterator enemyIterartor = theEnemies.begin(); enemyIterartor != theEnemies.end(); ++enemyIterartor)
				{
					if (score >=1000)
					{
						((*enemyIterartor)->setSpritePos({ (*enemyIterartor)->getSpritePos().x, (*enemyIterartor)->getSpritePos().y +10}));
					}
					
					
				 else if (score >= 500)
				 {
					 ((*enemyIterartor)->setSpritePos({ (*enemyIterartor)->getSpritePos().x,(*enemyIterartor)->getSpritePos().y + 5 }));
				 }
				 

				 else
				 {

					 ((*enemyIterartor)->setSpritePos({ (*enemyIterartor)->getSpritePos().x,(*enemyIterartor)->getSpritePos().y + 1 }));

				 }
				
				}
				
				
			}
			 
			
				


			
			
			
			

			
			
			

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/

		//compares eveything in the player vector with everything in the Enemy vector and checks if any are colliding
			if (theGameState == PLAYING)
			{
				for (vector<cPlayer*>::iterator playerIterartor = player.begin(); playerIterartor != player.end(); ++playerIterartor)
				{

					for (vector<cEnemy*>::iterator enemyIterator = theEnemies.begin(); enemyIterator != theEnemies.end(); ++enemyIterator)
					{
						if ((*enemyIterator)->collidedWith(&(*enemyIterator)->getBoundingRect(), &(*playerIterartor)->getBoundingRect()))
						{
							//if there is a collision load the end scene 
							
							theGameState = END;
							theSoundMgr->getSnd("crash")->play(0);

						}
						
					}
				}
			}
			
		}
	//adds 1 to the score
		if (theGameState == PLAYING)
		{
			score++;
			sScore = gameTextList[1] + to_string(score);
			theTextureMgr->deleteTexture("Score");
		}
	
}






bool cGame::getInput(bool theLoop)
{
	SDL_Event event;
	

		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
			{
				theLoop = false;
			}

			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
			
						theAreaClicked = { event.motion.x, event.motion.y };

				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					if (theGameState == PLAYING)
					{
						//Checks if the player has pressed the down arrow key and moves them down
						vector<cPlayer*>::iterator playerIterator = player.begin();
						for (vector<cPlayer*>::iterator playerIterartor = player.begin(); playerIterartor != player.end(); ++playerIterartor)
						{

							if ((*playerIterator)->getSpritePos().y < (renderHeight - (*playerIterator)->getSpritePos().h))
							{
								(*playerIterator)->setSpritePos({ (*playerIterator)->getSpritePos().x, (*playerIterator)->getSpritePos().y + 20 });
								

							}
						}
					}

				}
				break;

				case SDLK_UP:
				{
					if (theGameState == PLAYING)
					{
						//Checks if the player has pressed the up arrow key and moves them up
						vector<cPlayer*>::iterator playerIterator = player.begin();
						for (vector<cPlayer*>::iterator playerIterartor = player.begin(); playerIterartor != player.end(); ++playerIterartor)
						{

							if ((*playerIterator)->getSpritePos().y > (0))
							{
								(*playerIterator)->setSpritePos({ (*playerIterator)->getSpritePos().x, (*playerIterator)->getSpritePos().y - 20 });
								theSoundMgr->getSnd("carNoise")->play(0);
							}
						}
					}
				}
				break;
				case SDLK_RIGHT:
				{
					//Checks if the player has pressed the right arrow key and moves them right
					if (theGameState == PLAYING)
					{
						vector<cPlayer*>::iterator playerIterator = player.begin();
						for (vector<cPlayer*>::iterator playerIterartor = player.begin(); playerIterartor != player.end(); ++playerIterartor)
						{

							if ((*playerIterator)->getSpritePos().x < (renderWidth - (*playerIterator)->getSpritePos().w))
							{
								(*playerIterator)->setSpritePos({ (*playerIterator)->getSpritePos().x + 20, (*playerIterator)->getSpritePos().y });

							}
						}
					}
				}
				break;

				case SDLK_LEFT:
				{
					if (theGameState == PLAYING)
					{
						//Checks if the player has pressed the left arrow key and moves them left 
						vector<cPlayer*>::iterator playerIterator = player.begin();
						for (vector<cPlayer*>::iterator playerIterartor = player.begin(); playerIterartor != player.end(); ++playerIterartor)
						{

							if ((*playerIterator)->getSpritePos().x > (0))
							{
								(*playerIterator)->setSpritePos({ (*playerIterator)->getSpritePos().x - 20 , (*playerIterator)->getSpritePos().y });

							}
						}
					}
				}
				break;
				case SDLK_SPACE:
				{

				}

				break;
				default:
					break;
				}

			default:
				break;
			}

		}
	
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}