/*******************************************************************************
 * @file		utils/cli/cli.c
 * @brief		Command line interface module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.07	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <termios.h>
// System
#include "cli.h"

/* Private Variables ---------------------------------------------------------*/
static struct termios	old_tty;
static ptCLI_COMMAND	ptCmd;

/* Private Function Prototypes -----------------------------------------------*/
static void kbcleanup(void);
static int getch(void);
static void makeCommand(const char ch);
static void processCommand(const char* const cmd);

/* APIs ----------------------------------------------------------------------*/
void CLI_Init(const ptCLI_COMMAND cmd) {
	tcgetattr(STDIN_FILENO, &old_tty);
	struct termios	new_tty = old_tty;

	new_tty.c_lflag &= ~ICANON;	// disable line-at-a-time
	new_tty.c_lflag &= ~ECHO;		// disable echo
	new_tty.c_lflag &= ~ECHOE;	// ERASE, WERASE
	new_tty.c_cc[VMIN] = 0;			// don't wait for character
	new_tty.c_cc[VTIME] = 0;		// no minimum wait time

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_tty) == 0) {
		atexit(kbcleanup);	// restore terminal settings when the program exits.
	}
	else {
		perror("tcsetattr()");
		exit(1);
	}

	ptCmd = cmd;
}

void CLI_DisplayMenu(void) {
	puts("");
	puts("#-------------* Menu *-------------#");
	puts("#**********************************#");
	int i = 0;
	while (1) {
		if (ptCmd[i].cmd == NULL)
			break;
		printf(" %s%s\n", ptCmd[i].cmd, ptCmd[i].desc);
		++i;
	}
	puts("#**********************************#");
}

void CLI_GetCommand(void) {
	char ch = getch();
	if (ch != '\0') {
		makeCommand(ch);
	}
}

/* Private Functions ---------------------------------------------------------*/
static void kbcleanup(void) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_tty);	// restore
}

static int getch(void) {
	char ch;
	if (read(STDIN_FILENO, &ch, 1) == 0)
		return '\0';
	else
		return ch;
}

static void makeCommand(const char ch) {
	static char cmd[32];
	static int	idx = 0;

	if ((ch == '\r') || (ch == '\n')) {
		write(STDOUT_FILENO, &ch, 1);
		if (cmd[idx - 1] == 0x20)
			--idx;
		cmd[idx++] = '\0';
		processCommand(cmd);
		idx = 0;
	}
	else if (ch == 0x20) {		// <SPACE>
		write(STDOUT_FILENO, &ch, 1);
		if ((idx != 0) && (cmd[idx - 1] != 0x20))
			cmd[idx++] = ch;
	}
	else if (ch == 0x7f) {		// <BACKSPACE>
		char c = '\b';
		write(STDOUT_FILENO, &c, 1);
		c = ' ';
		write(STDOUT_FILENO, &c, 1);
		c = '\b';
		write(STDOUT_FILENO, &c, 1);
		if (idx > 0)
			--idx;
	}
	else {
		write(STDOUT_FILENO, &ch, 1);
		cmd[idx++] = ch;
	}
}

static void processCommand(const char* const cmd) {
	int i = 0;
	while (1) {
		if (ptCmd[i].cmd == NULL)
			break;

		int len = strlen(cmd) + 1;
		char name[8];
		int idx = 0;
		int j;
		for (j = 0; j < len; ++j) {
			if ((cmd[j] == ' ') || (cmd[j] == '\0')) {
				name[idx++] = '\0';
				idx = 0;
				break;
			}
			else {
				name[idx++] = cmd[j];
			}
		}

		int argc = 0;
		char args[8][8];
		char* argv[8];
		if (cmd[j] != '\0') {
			if (cmd[j] == 0x20)
				++j;

			for (int k = j; k < len; ++k) {
				if ((cmd[k] == ' ') || (cmd[k] == '\0')) {
					args[argc][idx++] = '\0';
					argv[argc] = args[argc];
					++argc;
					idx = 0;
				}
				else {
					args[argc][idx++] = cmd[k];
				}
			}
		}

		if (strcasecmp(name, ptCmd[i].cmd) == 0) {
			ptCmd[i].pfn(argc, argv);
		}
		++i;
	}
}
