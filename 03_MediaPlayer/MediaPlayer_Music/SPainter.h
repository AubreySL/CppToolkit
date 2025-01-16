#pragma once
#include "Common.h"
typedef struct SPainter
{
	SDL_Renderer* render;
	SDL_Surface* surface;
}SPainter;

void init_painter(SPainter* pthis, SDL_Renderer* render, SDL_Surface* surface);
void render_surface(SPainter* pthis, SDL_Rect* src, SDL_Rect* dst);
