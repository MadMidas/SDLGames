#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

//boolean to maintain program loop
bool quit = false;

// delta time init() - for frame rate ind
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0.0;

// set playewr speed
float playerSpeed = 500.0f;

//movement in the xaxis - up or down
float yDir;

//variables use to control integer creep
float pos_X, pos_Y;

//create rectangles for player graphic
SDL_Rect playerPos;

//enemy stuff
#include "enemy.h"
#include <vector>

//enemy vector list
vector<Enemy> enemyList;

//bullet info
#include "bullet.h"
vector<Bullet> bulletList;

//declare sounds
Mix_Chunk* laser;
Mix_Chunk* explosion;
Mix_Music* bgs;

void CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			//play laser sound
			Mix_PlayChannel(-1, laser, 0);
			bulletList[i].active = true;
			bulletList[i].posRect.y = (pos_Y + (playerPos.h / 2));
			bulletList[i].posRect.y = (bulletList[i].posRect.y - (bulletList[i].posRect.h / 2));

			bulletList[i].posRect.x = playerPos.x;

			bulletList[i].pos_Y = pos_Y;

			bulletList[i].pos_X = playerPos.x;
			break;
		}
	}
}

//variabl;es for score and lines
//score , lives and fonts
int playerScore, oldScore, playerLives, oldLives;

//declare font
TTF_Font* font;

//font color
SDL_Color colorP1 = { 0, 255, 0, 255 };

//Create surfaces
SDL_Surface* scoreSurface, * livesSurface;

//create textures
SDL_Texture* scoreTexture, * livesTexture;

SDL_Rect scorePos, livesPos;

string tempScore, tempLives;

void UpdateScore(SDL_Renderer* renderer)
{
	//create the text
	tempScore = "Player Score: " + std::to_string(playerScore);

	//create render text
	scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP1);

	//creating exture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);

	SDL_FreeSurface(scoreSurface);

	oldScore = playerScore;
}

void UpdateLives(SDL_Renderer* renderer)
{
	// creating the text
	tempLives = "Player Lives: " + std::to_string(playerLives);

	//create render text
	livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP1);

	//creating exture
	livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);

	SDL_QueryTexture(livesTexture, NULL, NULL, &livesPos.w, &livesPos.h);

	SDL_FreeSurface(livesSurface);

	oldLives = playerLives;
}

int main(int argc, char* argv[])
{
	SDL_Window* window; //Declare a pointer

	//create a render variable
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);

	//create an application window with the following settings:
	window = SDL_CreateWindow(
		"Space War",						//window title
		SDL_WINDOWPOS_UNDEFINED,			//initialize x position
		SDL_WINDOWPOS_UNDEFINED,			//initialize y position
		1024,								//width, in pizels
		768,								//height in pixes
		SDL_WINDOW_OPENGL					//flags - see below
	);

	//check tht the window was successfully created
	if (window == NULL)
	{
		//in the case that the window could not open
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//background image --CREATE

	//create a SDL surface
	SDL_Surface* surface = IMG_Load("./Assets/background.png");

	//create bkdg texture
	SDL_Texture* bkdg;

	//place surface into the texture
	bkdg = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface
	SDL_FreeSurface(surface);

	//createe rectabgles for the menu graphics
	SDL_Rect bkdgPos;

	// set bkdgPos x, y, width and height
	bkdgPos.x = 0;
	bkdgPos.y = 0;
	bkdgPos.w = 1024;
	bkdgPos.h = 768;

	//background image -- Create END

	//player image -- CREATE

	//create a SDL surface
	surface = IMG_Load("./Assets/player.png");

	//create bkdg texture
	SDL_Texture* player;

	//place surface into the texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface
	SDL_FreeSurface(surface);

	// set bkdgPos x, y, width and height
	playerPos.x = 10;
	playerPos.y = 360;
	playerPos.w = 111;
	playerPos.h = 69;

	//player image -- Create END

	//SDL event to handle input
	SDL_Event event;



	//creeate bullet list
	for (int i = 0; i < 10; i++)
	{
		Bullet tempBullet(renderer, i + 5, i + 5);
		bulletList.push_back(tempBullet);
	}

	//clear enemy list
	enemyList.clear();

	//fill enbemy list
	for (int i = 0; i < 8; i++)
	{
		Enemy tempEnemy(renderer);
		enemyList.push_back(tempEnemy);
	}

	//init audio playback
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//load laser sound
	laser = Mix_LoadWAV("./Assets/projectileSound.wav");

	//load explosion sound
	explosion = Mix_LoadWAV("./Assets/explosion.wav");

	//load background sound
	bgs = Mix_LoadMUS("./Assets/backgroundMusic.wav");

	//if the musio is not laying , play it and loop
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgs, -1);
	}

	//init player score, lives font
	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 5;

	TTF_Init();
	font = TTF_OpenFont("./Assets/retroFont.ttf", 20);

	//set up rectabgles
	scorePos.x = scorePos.y = 10;
	livesPos.x = 10;
	livesPos.y = 40;

	UpdateScore(renderer);
	UpdateLives(renderer);

	//setup our enum to handle all the game states
	enum GameState {GAME, WIN, LOSE};

	//variable to track where we are in the game
	GameState gameState = GAME;

	//bool values to allow movement through the individual states
	bool game, win, lose;

	//basic game loop
	while (!quit)
	{
		switch (gameState)
		{
		case GAME:
		{
			game = true;

			//load instructions background
			surface = IMG_Load("./Assets/background.png");

			//place surface into the texture
			bkdg = SDL_CreateTextureFromSurface(renderer, surface);

			//free the surface
			SDL_FreeSurface(surface);

			//start the game frsh
			enemyList.clear();

			for (int i = 0; i < 8; i++)
			{
				Enemy tempEnemy(renderer);
				enemyList.push_back(tempEnemy);
			}

			//reset player's score and lives
			playerScore = 0;
			playerLives = 3;

			while (game)
			{
				//create delta time
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;


				if (SDL_PollEvent(&event))
				{
					//close window by the windows X button
					if (event.type == SDL_QUIT)
					{
						quit = true;
					}

					switch (event.type)
					{
						/* look for a keypress */
					case SDL_KEYUP:
						//check the space bar
						switch (event.key.keysym.sym)
						{
						case SDLK_SPACE:
							CreateBullet();
							//pickupList[0].Reset();
							break;
						default:
							break;
						}
					}
				}//poll event END

				//player movement
				//get the keyboard state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

				//check which arrows are pressed
				if (currentKeyStates[SDL_SCANCODE_UP])
				{
					yDir = -1;
				}
				else if (currentKeyStates[SDL_SCANCODE_DOWN])
				{
					yDir = 1;
				}
				else
				{
					yDir = 0;
				}

				//UPDATE*******************************************

				// get new position to move player using directrions and deltatime
				pos_Y += (playerSpeed * yDir) * deltaTime;

				//move player
				playerPos.y = (int)(pos_Y + 0.5f);

				// keep player on screen
				//moving Up
				if (playerPos.y < 0)
				{
					playerPos.y = 0;
					pos_Y = 0;
				}

				// keep player on screen
				//moving down
				if (playerPos.y > 768 - playerPos.h)
				{
					playerPos.y = 768 - playerPos.h;
					pos_Y = 768 - playerPos.h;
				}

				//update bullets
				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						bulletList[i].Update(deltaTime);
					}
				}

				//update enemy list
				for (int i = 0; i < 8; i++)
				{
					enemyList[i].Update(deltaTime);
				}

				//check for collision of bullets and enemenies
				//for loop to croll throigh all the players bullets
				for (int i = 0; i < bulletList.size(); i++)
				{
					//check to see if thios bullet is activ in world
					if (bulletList[i].active == true)
					{
						//check all enemies against active bullets
						for (int j = 0; j < enemyList.size(); j++)
						{
							//see if there is a collision bewteen this bullet and this enemy usidng sdl
							if (SDL_HasIntersection(&bulletList[i].posRect, &enemyList[j].posRect))
							{

								//play explosion sound
								Mix_PlayChannel(-1, explosion, 0);

								//reset the enemy
								enemyList[j].Reset();

								//reset the bullet
								bulletList[i].Reset();

								//give player points
								playerScore += 10;

								//check see if player can go to lvl2
								if (playerScore >= 500)
								{
									game = false;
									gameState = WIN;
								}
							}
						}
					}
				}

				//check for collision with the other player and enemenies
				//check all enemies against active bullet
				for (int i = 0; i < enemyList.size(); i++)
				{
					//see if there is a collision bewteeen this bulet and this enbemy usinf sdl
					if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
					{
						//play explosion sound
						Mix_PlayChannel(-1, explosion, 0);

						//reset the enmy
						enemyList[i].Reset();

						//player loses life
						playerLives -= 1;

						//check to see if player loses
						if (playerLives <= 0)
						{
							game = false;
							gameState = LOSE;
						}
					}
				}

				//update score and lives on screen text
				if (playerScore != oldScore)
				{
					UpdateScore(renderer);
				}

				if (playerLives != oldLives)
				{
					UpdateLives(renderer);
				}

				//UPDATE*******************************************

				//DRAW++++++++++++++++++++++++++++++++++++

				//clear the old buffer
				SDL_RenderClear(renderer);

				//prepare to draw background
				SDL_RenderCopy(renderer, bkdg, NULL, &bkdgPos);

				//draw bullets
				for (int i = 0; i < bulletList.size(); i++)
				{
					bulletList[i].Draw(renderer);
				}

				//prepare to draw player
				SDL_RenderCopy(renderer, player, NULL, &playerPos);

				//draw enemy list
				for (int i = 0; i < 8; i++)
				{
					enemyList[i].Draw(renderer);
				}

				//draw score texture
				SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

				//draw lives texture
				SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);

				//draw new info to the screen
				SDL_RenderPresent(renderer);

				//END DRAW++++++++++++++++++++++++++++++++
			}//ends game screen
			break;
		}

		case WIN:
			win = true;

			std::cout << "The Game State is WIN" << endl;
			std::cout << "Press the R key to start the game" << endl;
			std::cout << "Press the Q key to Quit Game" << endl;
			std::cout << endl;

			//load instructions background
			//create sdl surface
			surface = IMG_Load("./Assets/winScreen.png");

			//place surface into the texture
			bkdg = SDL_CreateTextureFromSurface(renderer, surface);

			// free the surface
			SDL_FreeSurface(surface);

			//background image -- CREATE END

			while (win)
			{
				//create deltatime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;
				//check for input
				if (SDL_PollEvent(&event))
				{
					//close window by the windows X button
					if (event.type == SDL_QUIT)
					{
						quit = true;
						win = false;
						break;
					}

					switch (event.type)
					{
						/*look for a keypress*/
					case SDL_KEYUP:
						/*cjeck the SDLkey value and move change the coords*/
						switch (event.key.keysym.sym)
						{
						case SDLK_r:
							win = false;
							gameState = GAME;
							break;
						case SDLK_q:
							quit = true;
							win = false;
							break;
						default:
							break;
						}
					}
				}
				// DRAW++++++++++++++++++++++++++++++++++++++++++++
				//Draw section
				//clear the old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkdg1
				SDL_RenderCopy(renderer, bkdg, NULL, &bkdgPos);

				//draw new info to the screen
				SDL_RenderPresent(renderer);

				//END DRAW +++++++++++++++++++++++++++++++++++++++++++++++
			}//end win loop
			break;

		case LOSE:
			lose = true;

			std::cout << "The Game State is LOSE" << endl;
			std::cout << "Press the R key to start the game" << endl;
			std::cout << "Press the Q key to Quit Game" << endl;
			std::cout << endl;

			//load instructions background
			//create sdl surface
			surface = IMG_Load("./Assets/loseScreen.png");

			//place surface into the texture
			bkdg = SDL_CreateTextureFromSurface(renderer, surface);

			// free the surface
			SDL_FreeSurface(surface);

			//background image -- CREATE END

			while (lose)
			{
				//create deltatime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input
				if (SDL_PollEvent(&event))
				{
					//close window by the windows X button
					if (event.type == SDL_QUIT)
					{
						quit = true;
						lose = false;
						break;
					}

					switch (event.type)
					{
						/*look for a keypress*/
					case SDL_KEYUP:
						/*cjeck the SDLkey value and move change the coords*/
						switch (event.key.keysym.sym)
						{
						case SDLK_r:
							lose = false;
							gameState = GAME;
							break;
						case SDLK_q:
							quit = true;
							lose = false;
							break;
						default:
							break;
						}
					}
				}
				// DRAW++++++++++++++++++++++++++++++++++++++++++++
				//Draw section
				//clear the old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkdg1
				SDL_RenderCopy(renderer, bkdg, NULL, &bkdgPos);

				//draw new info to the screen
				SDL_RenderPresent(renderer);

				//END DRAW +++++++++++++++++++++++++++++++++++++++++++++++
			}//ends lose screen
			break;
		}//ends switch
	}//ends game loop

	////basic game loop
	//while (!quit)
	//{
		////create delta time
		//thisTime = SDL_GetTicks();
		//deltaTime = (float)(thisTime - lastTime) / 1000;
		//lastTime = thisTime;


		//if (SDL_PollEvent(&event))
		//{
		//	//close window by the windows X button
		//	if (event.type == SDL_QUIT)
		//	{
		//		quit = true;
		//	}

		//	switch (event.type)
		//	{
		//		/* look for a keypress */
		//	case SDL_KEYUP:
		//		//check the space bar
		//		switch (event.key.keysym.sym)
		//		{
		//		case SDLK_SPACE:
		//			CreateBullet();
		//			//pickupList[0].Reset();
		//			break;
		//		default:
		//			break;
		//		}
		//	}
		//}//poll event END

		////player movement
		////get the keyboard state
		//const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		////check which arrows are pressed
		//if (currentKeyStates[SDL_SCANCODE_UP])
		//{
		//	yDir = -1;
		//}
		//else if (currentKeyStates[SDL_SCANCODE_DOWN])
		//{
		//	yDir = 1;
		//}
		//else
		//{
		//	yDir = 0;
		//}

		////UPDATE*******************************************

		//// get new position to move player using directrions and deltatime
		//pos_Y += (playerSpeed * yDir) * deltaTime;

		////move player
		//playerPos.y = (int)(pos_Y + 0.5f);

		//// keep player on screen
		////moving Up
		//if (playerPos.y < 0)
		//{
		//	playerPos.y = 0;
		//	pos_Y = 0;
		//}

		//// keep player on screen
		////moving down
		//if (playerPos.y > 768 - playerPos.h)
		//{
		//	playerPos.y = 768 - playerPos.h;
		//	pos_Y = 768 - playerPos.h;
		//}

		////update bullets
		//for (int i = 0; i < bulletList.size(); i++)
		//{
		//	if (bulletList[i].active == true)
		//	{
		//		bulletList[i].Update(deltaTime);
		//	}
		//}

		////update enemy list
		//for (int i = 0; i < 8; i++)
		//{
		//	enemyList[i].Update(deltaTime);
		//}

		////check for collision of bullets and enemenies
		////for loop to croll throigh all the players bullets
		//for (int i = 0; i < bulletList.size(); i++)
		//{
		//	//check to see if thios bullet is activ in world
		//	if (bulletList[i].active == true)
		//	{
		//		//check all enemies against active bullets
		//		for (int j = 0; j < enemyList.size(); j++)
		//		{
		//			//see if there is a collision bewteen this bullet and this enemy usidng sdl
		//			if (SDL_HasIntersection(&bulletList[i].posRect, &enemyList[j].posRect))
		//			{

		//				//play explosion sound
		//				Mix_PlayChannel(-1, explosion, 0);

		//				//reset the enemy
		//				enemyList[j].Reset();

		//				//reset the bullet
		//				bulletList[i].Reset();

		//				//give player points
		//				playerScore += 10;

		//				//check see if player can go to lvl2
		//				if (playerScore >= 500)
		//				{
		//					//game = false;
		//					//gameState = LEVEL2;
		//				}
		//			}
		//		}
		//	}
		//}

		////check for collision with the other player and enemenies
		////check all enemies against active bullet
		//for (int i = 0; i < enemyList.size(); i++)
		//{
		//	//see if there is a collision bewteeen this bulet and this enbemy usinf sdl
		//	if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
		//	{
		//		//play explosion sound
		//		Mix_PlayChannel(-1, explosion, 0);

		//		//reset the enmy
		//		enemyList[i].Reset();

		//		//player loses life
		//		playerLives -= 1;

		//		//check to see if player loses
		//		if (playerLives <= 0)
		//		{
		//			//game = false;
		//			//gameState = LOSE;
		//		}
		//	}
		//}

		////update score and lives on screen text
		//if (playerScore != oldScore)
		//{
		//	UpdateScore(renderer);
		//}

		//if (playerLives != oldLives)
		//{
		//	UpdateLives(renderer);
		//}

		////UPDATE*******************************************

		////DRAW++++++++++++++++++++++++++++++++++++
		//
		////clear the old buffer
		//SDL_RenderClear(renderer);

		////prepare to draw background
		//SDL_RenderCopy(renderer, bkdg, NULL, &bkdgPos);

		////draw bullets
		//for (int i = 0; i < bulletList.size(); i++)
		//{
		//	bulletList[i].Draw(renderer);
		//}

		////prepare to draw player
		//SDL_RenderCopy(renderer, player, NULL, &playerPos);

		////draw enemy list
		//for (int i = 0; i < 8; i++)
		//{
		//	enemyList[i].Draw(renderer);
		//}

		////draw score texture
		//SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

		////draw lives texture
		//SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);

		////draw new info to the screen
		//SDL_RenderPresent(renderer);

		////END DRAW++++++++++++++++++++++++++++++++
	//}//end game loop

	//close and destroy the window
	SDL_DestroyWindow(window);

	//clean up
	SDL_Quit();

	return 0;
}
