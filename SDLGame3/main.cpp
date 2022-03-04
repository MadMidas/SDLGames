#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

//booolean to maintain program loop in
bool quit = false;

//deltatime inuit() fpor frame rate ind
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

//player movemenet - controlled by keyporessh
int playerMovement = 71;

//create rectangles for the menu
SDL_Rect playerPos;

// NEW PATROLLING 1 ********************************

//enemy include information
#include "enemy.h"
#include <vector>

//enemy list 
vector<Enemy> enemyList;

//declare max number of enemis
int numberOfEnemies;

// NEW PATROLLING 1 END ********************************

//NEW INVENTORY 1*****************************************

#include "coin.h";

//coin list variabe
vector<Coin> coinList;

//declaremax number of coins in
int numberOfCoins;

//declare sound 
Mix_Chunk* pickup;

//NEW INVENTORY 1 END*****************************************

int main(int argc, char* argv[]) {
	SDL_Window* window; //Declare a pointer

	//create a render variable
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);

	//create an application window with the following settings:
	window = SDL_CreateWindow(
		"Dungeon Crawler",						//window title
		SDL_WINDOWPOS_UNDEFINED,			//initialize x position
		SDL_WINDOWPOS_UNDEFINED,			//initialize y position
		642,								//width, in pizels
		358,								//height in pixes
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
	SDL_Surface* surface = IMG_Load("./Assets/level.png");

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
	bkdgPos.w = 642;
	bkdgPos.h = 358;

	//background image -- Create END

	//player imgae -- Create

	//create a sdl SDL_FreeSurface
	surface = IMG_Load("./Assets/player.png");

	//create player texture
	SDL_Texture* player;

	//place surface nto the SDL_Texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface 
	SDL_FreeSurface(surface);

	//set the player pos x , y , width, height
	playerPos.x = 291;
	playerPos.y = 291;
	playerPos.w = 59;
	playerPos.h = 59;

	//player imgae -- Create END

	//SDL event to handle input
	SDL_Event event;

	//maze settings
	const int mazeWidth = 9;
	const int mazeHeight = 5;

	//create array for the maze o = open w = wall p = player
	string maze[mazeHeight][mazeWidth] = {
		{"O","O","O","O","O","O","O","O","O"},
		{"O","W","O","W","W","W","O","W","O"},
		{"O","O","O","O","W","O","O","O","O"},
		{"O","W","O","W","W","W","O","W","O"},
		{"O","O","O","O","P","O","O","O","O"}
	};

	//player starting position in the maze row 5 column 5 - maze[4][4]
	int playerHorizontal = 4;
	int playerVertical = 4;

	int playerScore = 0;

	// NEW PATROLLING 2 ************************************************

	//clear enemy list 
	enemyList.clear();

	//fill enemy list 

	//init max number of enemies
	numberOfEnemies = 4;

	//enemy for the upper right area 
	Enemy tempEnemy(renderer, 71, 2, 2, "left", "CCW", 575, 7);
	//add to enemy list 
	enemyList.push_back(tempEnemy);

	//enemy for the upper left area 
	Enemy tempEnemy2(renderer, 71, 2, 2, "right", "CW", 7, 7);
	//add to enemy list 
	enemyList.push_back(tempEnemy2);

	//enemy for the lower right area 
	Enemy tempEnemy3(renderer, 71, 2, 2, "right","CW", 433, 149);
	//add to enemy list 
	enemyList.push_back(tempEnemy3);

	//enemy for the lower left area 
	Enemy tempEnemy4(renderer, 71, 2, 2, "up","CCW", 149, 291);
	//add to enemy list 
	enemyList.push_back(tempEnemy4);

	// NEW PATROLLING 2 END ************************************************

	//NEW INVENTORY 2 START ************************************************

	//clear coin list 
	coinList.clear();

	//fill coin list 

	//init max number of coins
	numberOfCoins = 4;

	//coins picked up
	int totalCoins = 0;

	//con for the upper right 
	Coin tempCoin(renderer, 18, 18);
	//add to coin list
	coinList.push_back(tempCoin);

	//con for the lower right 
	Coin tempCoin1(renderer, 18, 302);
	//add to coin list
	coinList.push_back(tempCoin1);

	//con for the upper left 
	Coin tempCoin2(renderer, 586, 18);
	//add to coin list
	coinList.push_back(tempCoin2);

	//con for the lower left 
	Coin tempCoin3(renderer, 586, 302);
	//add to coin list
	coinList.push_back(tempCoin3);

	//init audio playback
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//load laser sound 
	pickup = Mix_LoadWAV("./Assets/pickup.wav");

	//NEW INVENTORY 2 END ************************************************

	//basic progra,m loop
	while (!quit) {

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
				case SDLK_RIGHT:



					//check to see if the players potential horizonatal is within the mazes right side limit
					if ((playerHorizontal + 1) < mazeWidth)
					{
						//if the spot to thje right is open == "O"
						if (maze[playerVertical][playerHorizontal + 1] == "O")
						{
							//move the player in the maze array
							maze[playerVertical][playerHorizontal + 1] = "P";

							//move the player in the maze array
							maze[playerVertical][playerHorizontal] = "O";

							//increase player horizontal position
							playerHorizontal += 1;

							//increase players position
							playerPos.x += playerMovement;
						}
					}

					break;
				case SDLK_LEFT:

					//check to see if the players potential horizonatal is within the mazes right side limit
					if ((playerHorizontal - 1) >= 0)
					{
						//if the spot to thje right is open == "O"
						if (maze[playerVertical][playerHorizontal - 1] == "O")
						{
							//move the player in the maze array
							maze[playerVertical][playerHorizontal - 1] = "P";

							//move the player in the maze array
							maze[playerVertical][playerHorizontal] = "O";

							//increase player horizontal position
							playerHorizontal -= 1;

							//increase players position
							playerPos.x -= playerMovement;
						}
					}

					break;
				case SDLK_UP:

					//check to see if the players potential horizonatal is within the mazes right side limit
					if ((playerVertical - 1) >= 0)
					{
						//if the spot to thje right is open == "O"
						if (maze[playerVertical - 1][playerHorizontal] == "O")
						{
							//move the player in the maze array
							maze[playerVertical - 1][playerHorizontal] = "P";

							//move the player in the maze array
							maze[playerVertical][playerHorizontal] = "O";

							//increase player vertical position
							playerVertical -= 1;

							//increase players position
							playerPos.y -= playerMovement;
						}
					}

					break;
				case SDLK_DOWN:

					//check to see if the players potential horizonatal is within the mazes right side limit
					if ((playerVertical + 1) < mazeHeight)
					{
						//if the spot to thje right is open == "O"
						if (maze[playerVertical + 1][playerHorizontal] == "O")
						{
							//move the player in the maze array
							maze[playerVertical + 1][playerHorizontal] = "P";

							//move the player in the maze array
							maze[playerVertical][playerHorizontal] = "O";

							//increase player vertical position
							playerVertical += 1;

							//increase players position
							playerPos.y += playerMovement;
						}
					}

					break;
				default:
					break;
				}
			}
		}//poll event END

		//****************************************************************UPDATE START

		//update enemies
		for (int i = 0; i < numberOfEnemies; i++)
		{
			enemyList[i].Update(deltaTime);
		}

		//check for collison with the player and the enmy enemyList
		for (int i = 0; i < enemyList.size(); i++)
		{
			//see if ther is a collision between the player and this enemy 
			if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect)) 
			{
				cout << "You have lost!" << endl;
			}
		}

		//check for collision with all players and the coins 
		for (int i = 0; i < coinList.size(); i++)
		{
			//see if there is a collision between the player and this coin 
			if (SDL_HasIntersection(&playerPos, &coinList[i].posRect)) {
				//remove coin from
				coinList[i].RemoveFromScreen();

				//plau pickup sound
				Mix_PlayChannel(-1, pickup, 0);

				//add one con to total
				totalCoins++;

				playerScore++;

				//output some temporary feedback
				cout << "Score: " << playerScore << endl;

				if (totalCoins == 4)
				{
					cout << "You have won!" << endl;
				}
			}
		}

		//****************************************************************UPDATE END

		//******************************************************************START DRAW

		//clear the old buffer
		SDL_RenderClear(renderer);

		//prepare to draw background
		SDL_RenderCopy(renderer, bkdg, NULL, &bkdgPos);

		//draw player 
		SDL_RenderCopy(renderer, player, NULL, &playerPos);

		//Draw Coins ****************************************************************
		for (int i = 0; i < numberOfCoins; i++)
		{
			coinList[i].Draw(renderer);
		}

		//draw enemies 
		for (int i = 0; i < numberOfEnemies; i++)
		{
			enemyList[i].Draw(renderer);
		}

		//draw info to screen
		SDL_RenderPresent(renderer);

		//******************************************************************END DRAW
	}//Game Loop END

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}