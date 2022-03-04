#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
using namespace std;

class Coin
{
	public:
		bool active;

		SDL_Texture* texture;

		SDL_Rect posRect;

		Coin(SDL_Renderer* renderer, int startX, int startY);

		void RemoveFromScreen();

		void Draw(SDL_Renderer* renderer);

		~Coin();
};