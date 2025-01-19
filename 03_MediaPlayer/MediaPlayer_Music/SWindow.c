#include "SWindow.h"
#include "SPainter.h"
#include "SPlayerMusic.h"
void init_window(SWindow* pthis, const char* imgURL)
{
	//init
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	if (imgURL != NULL)
	{
		pthis->background = IMG_Load(imgURL);
		pthis->w = pthis->background->w;
		pthis->h = pthis->background->h;

	}
	else
	{
		pthis->background = NULL;
		pthis->w = WIDTH;
		pthis->h = HEIGHT;
	}
}

void create_window(SWindow* pthis, const char* title)
{
	pthis->window = SDL_CreateWindow(title, 100, 100, pthis->w, pthis->h, SDL_WINDOW_SHOWN);
	pthis->render = SDL_CreateRenderer(pthis->window, -1, SDL_RENDERER_SOFTWARE);
}

void render_window(SWindow* pthis)
{
	if (pthis->background != NULL)
	{
		SPainter painter;
		init_painter(&painter, pthis->render, pthis->background);
		SDL_Rect dst = { 0,0, pthis->w, pthis->h };
		render_surface(&painter, NULL, &dst);
	}
}

void event_loop(SWindow* pthis)
{
	SPlayerMucsic playermusic;
	init_playermusic(&playermusic);
	init_playList(&playermusic);
	bool isRunning = true;
	while (isRunning)
	{
		SDL_Event msg;
		if (SDL_WaitEvent(&msg) != 0)
		{
			switch (msg.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
	
			}
			event_playermusic(&playermusic, &msg);
		}

		//paint
		SDL_RenderClear(pthis->render);
		render_window(pthis);
		render_playermusic(pthis->render, &playermusic);
		SDL_RenderPresent(pthis->render);

		//data
		onEvent_playermusic(&playermusic);
	}
}

void destory_window(SWindow* pthis)
{
	if (pthis->background != NULL)
	{
		SDL_FreeSurface(pthis->background);
	}
	SDL_DestroyRenderer(pthis->render);
	SDL_DestroyWindow(pthis->window);
}
