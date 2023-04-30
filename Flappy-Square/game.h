#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "types.h"
#include "font.h"

// Divided by 2 due to render scale of 2
#define WINDOWWIDTH (640/2)
#define WINDOWHEIGHT (480/2)

// Game state helps us know when to exit the program.
enum GameState { PLAYING, GAMEOVER };
GameState CurrentGameState;

class Game
{
	
	private:
		SDL_Event e;
  	SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		
		std::vector<SDL_FPoint> points;
		std::vector<SDL_Rect> rects;

	public:
		Game(void);
	
		void Screen_Show(void);
		void Screen_Clear(void);
	
		void Screen_Putpixel(float x, float y);
		void Screen_Putrect(int32 x, int32 y, int32 w, int32 h);
		void Screen_Putrect(SDL_Rect rect);
		void Screen_Putchar(int32 x, int32 y, char charcode);
		void Screen_Putstring(int32 x, int32 y, std::string string);
		uint32 Game_Getinput();
};

Game::Game(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOWWIDTH*2, WINDOWHEIGHT*2, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Flappy Square");
	SDL_RenderSetScale(renderer, 2, 2);
}
  
void Game::Screen_Show(void)
{
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
  
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	for (auto& point : points)
	{
		SDL_RenderDrawPointF(renderer,point.x,point.y);
	}
	for (auto& rect : rects)
	{
		SDL_RenderDrawRect(renderer, &rect);
	}
	SDL_RenderPresent(renderer);
}

void Game::Screen_Putpixel(float x, float y)
{
	SDL_FPoint mypoint;
	mypoint.x = x;
	mypoint.y = y;
	points.push_back(mypoint);
}

void Game::Screen_Putrect(int32 x, int32 y, int32 w, int32 h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;
	rects.push_back(rect);
}

void Game::Screen_Putchar(int32 x, int32 y, char charcode) {
	unsigned char* bitmap = font8x8_basic[charcode];
	
	uint32 set;
	
	uint16 charx;
	uint16 chary;

	uint16 screenx = x;
	uint16 screeny = y;
	
	for (charx=0; charx < 8; charx++) {
    for (chary=0; chary < 8; chary++) {
      set = bitmap[charx] & 1 << chary;
      if (set != 0) {
				Screen_Putpixel(screenx, screeny);
			}
			screenx++;
			
		}
		screenx = x;
		screeny++;
  }
}

void Game::Screen_Putstring(int32 x, int32 y, std::string string) {
	uint32 relx;
	for (relx=0; relx < string.length(); relx++) {
    Screen_Putchar((relx*8)+x, y, string[relx]);
  }
}

void Game::Screen_Putrect(SDL_Rect rect)
{
	rects.push_back(rect);
}

uint32 Game::Game_Getinput()
{
	uint32 result = 0;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			CurrentGameState = GAMEOVER;
		}
		if (e.type == SDL_KEYDOWN)
		{
    	result = e.key.keysym.sym;
		}

	}
	return result;
}


void Game::Screen_Clear()
{
	points.clear();
	rects.clear();
}