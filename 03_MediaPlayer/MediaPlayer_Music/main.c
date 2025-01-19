#include "SWindow.h"

int main(int argc, char* argv[])
{
	SWindow w;
	init_window(&w, "./images/background.jpg");
	create_window(&w, "MusicPlayer");
	event_loop(&w);
	destory_window(&w);
	return 0;

}