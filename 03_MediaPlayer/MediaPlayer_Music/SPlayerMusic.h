#pragma once

#include "Common.h"
#include "SButtonEx.h"

enum Player {Reduce, Pre, Play, Next, Add};
typedef struct SPlayerMucsic
{
	SButtonEx group[5];
	int max_count;

	Mix_Music* music;
	char str[10][20];
	int index;
	int maxIndex;

	TTF_Font* font;
	SDL_Surface* musicName;

}SPlayerMucsic;

void init_playermusic(SPlayerMucsic* pthis);
void render_playermusic(SDL_Renderer* render, SPlayerMucsic* pthis);
int event_playermusic(SPlayerMucsic* pthis, SDL_Event* msg);
void onEvent_playermusic(SPlayerMucsic* pthis);

void init_playList(SPlayerMucsic* pthis);