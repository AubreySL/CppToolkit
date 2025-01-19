#include "SPlayerMusic.h"
#include "SPainter.h"
void init_playermusic(SPlayerMucsic* pthis)
{
	pthis->max_count = 5;
	init_buttonEx(pthis->group + Reduce, 30, 300, Rect, "./images/volume-down-hover.png", "./images/volume-down.png");
	init_buttonEx(pthis->group + Pre, 80, 300, Rect, "./images/go-start-hover.png", "./images/go-start.png");
	init_buttonEx(pthis->group + Play, 130, 300, Circle, "./images/play-hover.png", "./images/play.png");
	init_buttonEx(pthis->group + Next, 180, 300, Rect, "./images/go-end-hover.png", "./images/go-end.png");
	init_buttonEx(pthis->group + Add, 230, 300, Rect, "./images/volume-up-hover.png", "./images/volume-up.png");

}

void render_playermusic(SDL_Renderer* render, SPlayerMucsic* pthis)
{
	for (int i = 0; i < pthis->max_count; ++i) {
		render_buttonEx(render, pthis->group + i);
	}
	//renderer sing name 
	SDL_Color color = { 0xd0,0x02,0x1b,0 };//#d0021b
	pthis->musicName = TTF_RenderUTF8_Blended(pthis->font, pthis->str[pthis->index], color);
	SPainter painter;
	init_painter(&painter, render, pthis->musicName);
	SDL_Rect dst = { 80, 250, pthis->musicName->w, pthis->musicName->h };
	render_surface(&painter, NULL, &dst);
	SDL_FreeSurface(pthis->musicName);
}

int event_playermusic(SPlayerMucsic* pthis, SDL_Event* msg)
{
	for (int i = 0; i < pthis->max_count; ++i) {
		if (event_buttonEx(pthis->group + i, msg) == true)
		{
			return i;
		}
	}
	return -1;
}

void onEvent_playermusic(SPlayerMucsic* pthis)
{
	int pos = -1;
	for (int i = 0; i < pthis->max_count; ++i)
	{
		if (isClicked_buttonEx(pthis->group + i) == true)
		{
			pos = i;
			break;
		}
	}
	int volume_val = Mix_GetMusicVolume(pthis->music);
	if (pos != -1)
	{
		switch (pos)
		{
		case Reduce:
			if (volume_val - 5 > 0)
			{
				Mix_VolumeMusic(volume_val -= 5);
			}
			break;
		case Pre:
			if (pthis->index == 0)
			{
				pthis->index = pthis->maxIndex - 1;
			}
			else
			{
				pthis->index--;
			}
			pthis->music = Mix_LoadMUS(pthis->str[pthis->index]);
			Mix_PlayMusic(pthis->music, -1);
			break;
		case Play:
		{
			static bool flag = true;
			if (flag == true)
			{
				set_image_buttonEx(pthis->group + Play, "./images/pause-one-hover.png", "./images/pause-one.png");
				flag = false;
				Mix_PauseMusic();
			}
			else {
				set_image_buttonEx(pthis->group + Play, "./images/play-hover.png", "./images/play.png");
				flag = true;
				Mix_ResumeMusic();
			}
			break;
		}
		case Next:
			if (pthis->index == pthis->maxIndex - 1)
			{
				pthis->index = 0;
			}
			else {
				pthis->index++;
			}
			pthis->music = Mix_LoadMUS(pthis->str[pthis->index]);
			Mix_PlayMusic(pthis->music, -1);
			break;
		case Add:
			if (volume_val + 5 < MIX_MAX_VOLUME)
			{
				Mix_VolumeMusic(volume_val += 5);
			}
			break;
		default:
			break;
		}
	}
}

void init_playList(SPlayerMucsic* pthis)
{
	pthis->index = 0;
	pthis->maxIndex = 2;
	strcpy_s(pthis->str[0], 20, "./temp/1.mp3");
	strcpy_s(pthis->str[1], 20, "./temp/2.wav");

	pthis->font = TTF_OpenFont("./temp/yahei.ttf", 25);
	pthis->music = Mix_LoadMUS(pthis->str[pthis->index]);
	Mix_PlayMusic(pthis->music, -1);
}
