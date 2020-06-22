#include "Matrix.hpp"
#include <curses.h>
#include "Core.hpp"

int main()
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	scrollok(stdscr, TRUE);
	idlok(stdscr, TRUE);

	Core core;

	core.Loop();

	endwin();
}
