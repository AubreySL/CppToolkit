#pragma once
#include "Common.h"

typedef struct SWindow
{
	SDL_Window* window;
	SDL_Renderer* render;
	int w;
	int h;
	SDL_Surface* background;
}SWindow;

void init_window(SWindow* pthis, const char* imgURL);
void create_window(SWindow* pthis, const char* title);
void render_window(SWindow* pthis);
void event_loop(SWindow* pthis);
void destory_window(SWindow* pthis);