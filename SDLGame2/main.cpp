#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

// boolean to maintain program loop 
bool quit = false;

// deltaTime init() - for frame rate ind. 
float deltaTime = 0.0f; 
int thisTime = 0; 
int lastTime = 0;

// vars for player's angle 
float x, y, playerAngle, oldAngle;

//float playerAngle = 90; for player pointing down

// for precision loss 
float pos_X, pos_Y;

// floats for direction 
float xDir, xDirOld; float yDir, yDirOld;

// player speed 
float playerSpeed = 400.0f;

// create rectangles for the menu graphics 
SDL_Rect playerPos;

// player's center point 
SDL_Point center;

//bullets 
#include <vector>
#include "Bullet.h"

vector<Bullet> bulletList;

//declare sounds
Mix_Chunk* laser;

void CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			Mix_PlayChannel(-1, laser, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = pos_X;

			bulletList[i].posRect.y = pos_Y;

			bulletList[i].pos_X = pos_X;

			bulletList[i].pos_Y = pos_Y;

			bulletList[i].xDir = xDirOld;

			bulletList[i].yDir = yDirOld;

			bulletList[i].Reposition();

			break;
		}
	}
}

#include <iostream>
#include <cstdlib>
using namespace std;

//large rocks
#include "largeRock.h"
vector<LargeRock> largeRockList;

//small rocks
#include "smallRock.h"
vector<SmallRock> smallRockList;

//declare sounds
Mix_Chunk* explosion;

int main(int argc, char* argv[]) {

	//seed random numbers
	srand(time(NULL));

	SDL_Window* window; // Declare a pointer
	// create a renderer variable 
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);// Initialize SDL2

	// Create an application window with the following settings: 
	window = SDL_CreateWindow(
	"Space Rocks",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1024,
		768,
		SDL_WINDOW_OPENGL
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
	SDL_Surface* surface = IMG_Load("./Assets/Background.png");

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

	//background image --CREATE END

	//Player image ----CREATE

	//create a sdl surface
	surface = IMG_Load("./Assets/player.png");

	//create bkdg texture
	SDL_Texture* player;

	//place surface in to the texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface
	SDL_FreeSurface(surface);

	//set playerPos x, y, width and height
	playerPos.x = 1024 / 2;
	playerPos.y = 768 / 2;
	playerPos.w = 49;
	playerPos.h = 32;

	//center of the layers s[prite
	center.x = playerPos.w / 2;
	center.y = playerPos.h / 2;

	//pos x and posy for precision losss
	pos_X = playerPos.x;
	pos_Y = playerPos.y;

	//the player graphic is facing right, tso the xDirold is set to 1 so bullets go in correct direction
	xDir = 1;
	yDir = 0;

	//the player graphic is facing right, tso the xDirold is set to 1 so bullets go in correct direction
	xDirOld = 1;
	yDirOld = 0;

	//PLayer image CREATE--- END

	//SDL even to handle input
	SDL_Event event;

	//create bullet list
	for (int i = 0; i < 10; i++)
	{
		Bullet tempBullet(renderer, -1000.0f, -1000.0f);

		bulletList.push_back(tempBullet);
	}

	////create large rock list
	//for (int i = 0; i < 1; i++)
	//{
	//	LargeRock tempRock(renderer, -1000.0f, -1000.0f);

	//	largeRockList.push_back(tempRock);
	//}

	////create small rock list
	//for (int i = 0; i < 2; i++)
	//{
	//	SmallRock tempRock(renderer, -1000.0f, -1000.0f);

	//	smallRockList.push_back(tempRock);
	//}

	//init audio playback
	Mix_OpenAudio(4410, MIX_DEFAULT_FORMAT, 2, 2048);

	//load laser sound
	laser = Mix_LoadWAV("./Assets/laser.wav");

	//load explosion sound
	explosion = Mix_LoadWAV("./Assets/explosion.wav");

	//level up****************************

	//rocks t5o start game
	int numberOfLargeRocks = 1;
	int numberOfSmallRocks = 2;

	//total rocks on the level
	int totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;

	//total rocks currently destyroyed
	int totalRocksDestroyed = 0;

	//counter for level
	int level = 0;

	//counter for score
	int score = 0;




	//level up****************************

	//create large rock list
	for (int i = 0; i < numberOfLargeRocks; i++)
	{
		LargeRock tempRock(renderer, -1000.0f, -1000.0f);

		largeRockList.push_back(tempRock);
	}

	//create small rock list
	for (int i = 0; i < numberOfSmallRocks; i++)
	{
		SmallRock tempRock(renderer, -1000.0f, -1000.0f);

		smallRockList.push_back(tempRock);
	}

	//activate all large rocks for level 1
	for (int i = 0; i < numberOfLargeRocks; i++)
	{
		largeRockList[i].Reposition();
	}
	std::cout << "Level 1" << endl;
	//basic game loop
	while (!quit)
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
			}

			switch (event.type)
			{
				//look for key press and release
			case SDL_KEYUP:
				//check for the SDLkey values
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					CreateBullet();
					break;

				//case SDLK_z:
				//	largeRockList[0].Reposition();
				//	break;

				//case SDLK_s:
				//	for (int i = 0; i < 2; i++)
				//	{
				//		if (smallRockList[i].active == false)
				//		{
				//			smallRockList[i].Reposition(largeRockList[0].posRect.x, largeRockList[0].posRect.y);
				//		}
				//	}
				//	//clear rock list
				//	largeRockList.clear();
				//	break;

				//case SDLK_a:
				//	//clear large rock
				//	largeRockList.clear();

				//	//clear small rock
				//	smallRockList.clear();

				//	for (int i = 0; i < 1; i++)
				//	{
				//		LargeRock tempRock(renderer, -1000.0f, -1000.0f);
				//		largeRockList.push_back(tempRock);
				//	}

				//	for (int i = 0; i < 1; i++)
				//	{
				//		SmallRock tempRock(renderer, -1000.0f, -1000.0f);
				//		smallRockList.push_back(tempRock);
				//	}
				//	break;

				default:
					break;
				}
			}
		}

		//player movement
		//get the keyboard state 
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		//check which arrows are pressed for xDir
		if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			//if left button is pressed set xdir to -1
			xDir = -1.0f;
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			//if right button is pressed set xdir to 1
			xDir = 1.0f;
		}
		else
		{
			//if neither presss 
			xDir = 0.0f;
		}

		//check which arrows are pressed for yDir
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			//if left button is pressed set ydir to -1
			yDir = -1.0f;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			//if right button is pressed set ydir to 1
			yDir = 1.0f;
		}
		else
		{
			//if neither presss 
			yDir = 0.0f;
		}

		//START UPDATE*************************

		//player update moving
		if (xDir != 0 || yDir != 0)
		{
			//get x and y
			x = playerPos.x - xDir;
			y = playerPos.y - yDir;

			//calculate angle
			playerAngle = atan2(yDir, xDir) * 180 / 3.14;

			//update old angle
			oldAngle = playerAngle;

			//update old direction 
			xDirOld = xDir;
			yDirOld = yDir;
		}
		else
		{
			//update ol angle
			oldAngle = playerAngle;
		}

		//get players nerw position
		pos_X += (playerSpeed * xDir) * deltaTime;
		pos_Y += (playerSpeed * yDir) * deltaTime;

		//adjust for precision loss
		playerPos.x = (int)(pos_X + 0.5f);
		playerPos.y = (int)(pos_Y + 0.5f);

		//player wrapping ----------------------------------------------
		//check to see if the player is off the left of the creen
		if (playerPos.x < (0 - playerPos.w))
		{
			playerPos.x = 1024;
			pos_X = playerPos.x;
		}

		//check to see if the player is off the right of the creen
		if (playerPos.x > 1024)
		{
			playerPos.x = (0 - playerPos.w);
			pos_X = playerPos.x;
		}

		//check to see if the player is off the top of the screen
		if (playerPos.y < (0 - playerPos.w))
		{
			playerPos.y = 768;
			pos_Y = playerPos.y;
		}

		//check to see if the player is off the top of the screen
		if (playerPos.y > 768)
		{
			playerPos.y = (0 - playerPos.w);
			pos_Y = playerPos.y;
		}
		//player wrapping end ----------------------------------------
		

		//update bullets in buletlist
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				bulletList[i].Update(deltaTime);
			}
		}

		//update rocks in largeRocklist
		for (int i = 0; i < largeRockList.size(); i++)
		{
			if (largeRockList[i].active == true)
			{
				largeRockList[i].Update(deltaTime);
			}
		}

		//update rocks in smallRocklist
		for (int i = 0; i < smallRockList.size(); i++)
		{
			if (smallRockList[i].active == true)
			{
				smallRockList[i].Update(deltaTime);
			}
		}

		//WRAPPING (COLLISION DETECTION)*****************************

		//check for collision of bullets with large riocks
		//for loop to scroll through all the players bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			//check to see id the bullet is actibve in the world
			if (bulletList[i].active == true)
			{
				//check all large rocks against active bullet
				for (int j = 0; j < largeRockList.size(); j++)
				{
					//see if there is a collision betywee this bullet and this enemy
					if (SDL_HasIntersection(&bulletList[i].posRect, &largeRockList[j].posRect))
					{
						//play explosion sound
						Mix_PlayChannel(-1, explosion, 0);

						score += 50;
						std::cout << "Score:\t" << score << endl;

						//LEVELUP**************************************

						//need two small rocks that are false as the number of small rocks grows
						int smallRockCounter = 0;

						//LEVELUP**************************************

						//create 2 small rocks
						for (int i = 0; i < smallRockList.size(); i++)
						{
							if (smallRockList[i].active == false)
							{
								smallRockList[i].Reposition(largeRockList[j].posRect.x, largeRockList[j].posRect.y);
								//increase small rock counter
								smallRockCounter++;
							}

							//once we find 2 inactive rocks exit the loop
							if (smallRockCounter == 2)
							{
								break;
							}
						}

						//reset the rock
						largeRockList[j].Deactivate();

						//reset the bullet
						bulletList[i].Deactivate();

						totalRocksDestroyed++;
					}
				}
			}
		}

		//check for collision of bullets with small riocks
		//for loop to scroll through all the players bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			//check to see id the bullet is actibve in the world
			if (bulletList[i].active == true)
			{
				//check all large rocks against active bullet
				for (int j = 0; j < smallRockList.size(); j++)
				{
					//see if there is a collision betywee this bullet and this enemy
					if (SDL_HasIntersection(&bulletList[i].posRect, &smallRockList[j].posRect))
					{
						//play explosion sound
						Mix_PlayChannel(-1, explosion, 0);

						score += 100;
						std::cout << "Score:\t" << score << endl;

						//reset the rock
						smallRockList[j].Deactivate();

						//reset the bullet
						bulletList[i].Deactivate();

						//levelup*********************************************
						//add one to total destroyed
						totalRocksDestroyed++;

						//check to see if all the rocks destryoyed
						if (totalRocksDestroyed >= totalNumberOfRocks)
						{
							//reset total rocks to 0
							totalRocksDestroyed = 0;

							//add rocks to level
							numberOfLargeRocks++;
							numberOfSmallRocks +=2 ;

							totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;

							//clear large rock list
							largeRockList.clear();

							//clear small rock list
							smallRockList.clear();

							level++;
							std::cout << "Now in level " << level << endl;
							//rebuild rock list with new numbewrs
							for (int i = 0; i < numberOfLargeRocks; i++)
							{
								LargeRock tempRock(renderer, -1000.0f, -1000.0f);

								largeRockList.push_back(tempRock);
							}

							//rebuild rock list with new numbewrs
							for (int i = 0; i < numberOfSmallRocks; i++)
							{
								SmallRock tempRock(renderer, -1000.0f, -1000.0f);

								smallRockList.push_back(tempRock);
							}

							//activate all large rocks for this level
							for (int i = 0; i < numberOfLargeRocks; i++)
							{
								largeRockList[i].Reposition();
							}


						}
					}
				}
			}
		}


		//WRAPPING (COLLISION DETECTION)*****************************

		//UPDATE END *****************************

		//START DRAW****************************

		//Draw Section
		//clear the old buffer
		SDL_RenderClear(renderer);

		//draw bkdg
		SDL_RenderCopy(renderer, bkdg, NULL, &bkdgPos);

		//draw bullets in bulletlist
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				bulletList[i].Draw(renderer);
			}
		}

		//draw largeRock in largerocklist
		for (int i = 0; i < largeRockList.size(); i++)
		{
			if (largeRockList[i].active == true)
			{
				largeRockList[i].Draw(renderer);
			}
		}

		//draw smallRock in smallrocklist
		for (int i = 0; i < smallRockList.size(); i++)
		{
			if (smallRockList[i].active == true)
			{
				smallRockList[i].Draw(renderer);
			}
		}

		//draw player
		SDL_RenderCopyEx(renderer, player, NULL, &playerPos, playerAngle, &center, SDL_FLIP_NONE);

		//draw new info to the screen
		SDL_RenderPresent(renderer);

		//END DRAW*********************
	}

	//close and destroy the window
	SDL_DestroyWindow(window);

	//clean up
	SDL_Quit();


	return 0;
}