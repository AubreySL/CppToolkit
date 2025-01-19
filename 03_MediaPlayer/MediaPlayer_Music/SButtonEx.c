#include "SButtonEx.h"
#include "SPainter.h"

void init_buttonEx(SButtonEx* pthis, int x, int y,enum ButtonType type, const char* hover_imgURL, const char* bk_imgURL)
{
	pthis->dst = (SDL_Rect){ x,y,BUTTON_W, BUTTON_H };
	pthis->type = type;
	pthis->isClicked = false;
	pthis->isHover= false;
	pthis->hover_surface = IMG_Load(hover_imgURL);
	pthis->bk_surface = IMG_Load(bk_imgURL);
}

void set_pos_button(SButtonEx* pthis, int x, int y)
{
	pthis->dst = (SDL_Rect){ x,y,BUTTON_W, BUTTON_H };
}

void set_hover_image(SButtonEx* pthis, const char* hover_imgURL)
{
	pthis->hover_surface = IMG_Load(hover_imgURL);
}

void set_bk_image(SButtonEx* pthis, const char* bk_imgURL)
{
	pthis->bk_surface = IMG_Load(bk_imgURL);
}

void set_image_buttonEx(SButtonEx* pthis, const char* hover_imgURL, const char* bk_imgURL)
{
	pthis->hover_surface = IMG_Load(hover_imgURL);
	pthis->bk_surface = IMG_Load(bk_imgURL);
}

void render_buttonEx(SDL_Renderer* render, SButtonEx* pthis)
{
	SPainter painter;
	SDL_Surface* temp = pthis->isHover ? pthis->hover_surface : pthis->bk_surface;
	init_painter(&painter, render, temp);
	render_surface(&painter, NULL, &pthis->dst);
}

bool hover_buttonEx(SButtonEx* pthis, SDL_Event* msg)
{
	int x = msg->motion.x;
	int y = msg->motion.y;
	if (pthis->type == Rect)
	{
		int startx = pthis->dst.x;
		int starty = pthis->dst.y;

		int endx = startx + BUTTON_W;
		int endy = starty + BUTTON_H;

		if (x >= startx && x <= endx && y >= starty && y <= endy)
		{
			pthis->isHover = true;
			return true;
		}
	}
	else if(pthis->type == Circle)
	{
		int x2 = pthis->dst.x + BUTTON_W / 2;
		int y2 = pthis->dst.y + BUTTON_H / 2;
		int r = BUTTON_W / 2;
		if (SDL_pow(1.0 * x - x2, 2) + SDL_pow(1.0 * y - y2, 2) <= SDL_pow(r, 2)) {
			pthis->isHover = true;
			return true;
		}
	}
	pthis->isHover = false;
	return false;
}

bool event_buttonEx(SButtonEx* pthis, SDL_Event* msg)
{
	bool isHover = hover_buttonEx(pthis, msg);
	if (isHover && msg->type == SDL_MOUSEBUTTONDOWN && msg->button.button == SDL_BUTTON_LEFT)
	{
		pthis->isClicked = true;
		return true;
	}
	pthis->isClicked = false;
	return false;
}

bool isClicked_buttonEx(SButtonEx* pthis)
{
	return pthis->isClicked;
}
