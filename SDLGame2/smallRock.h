#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;

class SmallRock {
public:
	bool active;

	SDL_Texture* texture;

	SDL_Rect posRect;

	float xDir, yDir;

	float speed;

	float pos_X, pos_Y;

	SDL_Point rockCenter;

	float rockAngle;

	SmallRock(SDL_Renderer* renderer, float x, float y);

	void Reposition(float x, float y);

	void Update(float deltaTime);

	void Draw(SDL_Renderer* renderer);

	void Deactivate();

	~SmallRock();


};