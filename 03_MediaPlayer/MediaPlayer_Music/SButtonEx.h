#pragma once
#include "Common.h"
enum ButtonType{Circle,Rect };
typedef struct SButtonEx
{
	SDL_Rect dst;
	enum ButtonType type;

	bool isHover;
	bool isClicked;

	SDL_Surface* hover_surface;
	SDL_Surface* bk_surface;
	
}SButtonEx;

void init_buttonEx(SButtonEx* pthis, int x, int y,
	enum ButtonType type, const char* hover_imgURL, const char* bk_imgURL);
void set_pos_button(SButtonEx* pthis, int x, int y);
void set_hover_image(SButtonEx* pthis, const char* hover_imgURL);
void set_bk_image(SButtonEx* pthis, const char* bk_imgURL);

void set_image_buttonEx(SButtonEx* pthis, const char* hover_imgURL, const char* bk_imgURL);

void render_buttonEx(SDL_Renderer* render, SButtonEx* pthis);
bool hover_buttonEx(SButtonEx* pthis, SDL_Event* msg);
bool event_buttonEx(SButtonEx* pthis, SDL_Event* msg);

bool isClicked_buttonEx(SButtonEx* pthis);