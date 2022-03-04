#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
using namespace std;

class Enemy
{
	public:
	int enemyMovement;

	SDL_Texture* texture;

	SDL_Rect posRect;

	int maxHorizontalMovement;
	int maxVerticalMovement;

	int horizontalCounter = 0;
	int verticalCounter = 0;

	string currentDirection;

	string enemyType;

	unsigned int lastTime, currentTime;

	Enemy(SDL_Renderer* renderer, int speed, int maxH, int maxV, string dir, string type, int startX, int startY);

	void Update(float deltaTime);

	void Draw(SDL_Renderer * renderer);

	void Reset();

	~Enemy();
};