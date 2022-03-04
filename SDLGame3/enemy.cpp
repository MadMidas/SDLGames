#include "enemy.h"
#include <string>

Enemy::Enemy(SDL_Renderer* renderer, int speed, int maxH, int maxV, string dir, string type, int startX, int startY)
{
	enemyMovement = speed; //set up pixels to move left right up down

	maxHorizontalMovement = maxH; //max number of moves the nemy can take left or right

	maxVerticalMovement = maxV; //max number of mobves an ebnemy can make up or down

	currentDirection = dir; //this is the directionm the enmy is currently moving

	enemyType = type; //this states if the ebenmy moves in a clockwise or counter clockwise directionm

	SDL_Surface* surface = IMG_Load("./Assets/enemy.png");

	texture = SDL_CreateTextureFromSurface(renderer,surface);

	SDL_FreeSurface(surface);

	posRect.x = startX; //the starting x position of the enemy
	posRect.y = startY; //the starting y position of the enemy

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL ,&w, &h);

	posRect.w = w;
	posRect.h = h;

	lastTime = 0; //int used to hold the latest value of current time
}

void Enemy::Update(float deltaTime) {
	//move once per 1/2 second - 1000 equals 1 second
	//update current time to the latest time elapsed
	currentTime = SDL_GetTicks();

	//check top see if enough timehas passes 
	if (currentTime > lastTime + 500)
	{
		if ((currentDirection == "left") && (horizontalCounter < maxHorizontalMovement))
		{
			posRect.x -= enemyMovement; //move enmy left one

			horizontalCounter++; //add to the counter tracking times hthe enemy has moved left

			if (horizontalCounter >= maxHorizontalMovement) //if the enemy has moved its allloted spaces
			{
				horizontalCounter = 0; //reset the moveemnt counter to 0

				if (enemyType == "CW") //if the enemy is emant to be moving clockwise
				{
					currentDirection = "up";
				}
				else if (enemyType == "CCW") //if the enemy is to be moving counter-clockwise
				{
					currentDirection = "down";
				}
			}
		}
		else if ((currentDirection == "down") && (verticalCounter < maxVerticalMovement))
		{
			posRect.y += enemyMovement; //move enmy left one

			verticalCounter++; //add to the counter tracking times hthe enemy has moved left

			if (verticalCounter >= maxVerticalMovement) //if the enemy has moved its allloted spaces
			{
				verticalCounter = 0; //reset the moveemnt counter to 0

				if (enemyType == "CW") //if the enemy is emant to be moving clockwise
				{
					currentDirection = "left";
				}
				else if (enemyType == "CCW") //if the enemy is to be moving counter-clockwise
				{
					currentDirection = "right";
				}
			}
		}
		else if ((currentDirection == "right") && (horizontalCounter < maxHorizontalMovement)) 
		{
			posRect.x += enemyMovement; //move enmy left one

			horizontalCounter++; //add to the counter tracking times hthe enemy has moved left

			if (horizontalCounter >= maxHorizontalMovement) //if the enemy has moved its allloted spaces
			{
				horizontalCounter = 0; //reset the moveemnt counter to 0

				if (enemyType == "CW") //if the enemy is emant to be moving clockwise
				{
					currentDirection = "down";
				}
				else if (enemyType == "CCW") //if the enemy is to be moving counter-clockwise
				{
					currentDirection = "up";
				}
			}
		}
		else if ((currentDirection == "up") && (verticalCounter < maxVerticalMovement))
		{
			posRect.y -= enemyMovement; //move enmy left one

			verticalCounter++; //add to the counter tracking times hthe enemy has moved left

			if (verticalCounter >= maxVerticalMovement) //if the enemy has moved its allloted spaces
			{
				verticalCounter = 0; //reset the moveemnt counter to 0

				if (enemyType == "CW") //if the enemy is emant to be moving clockwise
				{
					currentDirection = "right";
				}
				else if (enemyType == "CCW") //if the enemy is to be moving counter-clockwise
				{
					currentDirection = "left";
				}
			}
		}

		lastTime = currentTime; //update clock

	}
}

void Enemy::Draw(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

Enemy::~Enemy()
{
	//SDL_DestroyTexture(texture);
}