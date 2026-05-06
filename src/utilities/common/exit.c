// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <yyjson.h>

#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/utilities/common.h>

bool reymm_exit() {
	struct termios oldt, newt;
	char ch;
	int nread;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	newt.c_cc[VMIN]  = 0;  /* no esperar caracteres mínimos */
	newt.c_cc[VTIME] = 0;  /* timeout = 0, retornar inmediatamente */

	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	nread = read(STDIN_FILENO, &ch, 1);
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	if (nread <= 0) return false;  /* sin input, seguir iterando */
	return ch == 'e' || ch == 'E';
}