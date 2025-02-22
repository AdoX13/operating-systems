#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils/utils.h"

#define MAX_LINE_SIZE		256
#define MAX_ARGS		8

#define ERROR			0
#define SIMPLE			1
#define REDIRECT		2
#define PIPE			3
#define EXIT_CMD		5

static char *verb;
static char **args;
static char *stdin_file;
static char *stdout_file;
static char *stderr_file;

static int type;


static void alloc_mem(void)
{
	args = malloc(MAX_ARGS * sizeof(char *));
}

static int parse_line(char *line)
{
	int ret = SIMPLE;
	int idx = 0;
	char *token;
	char *delim = "=\n";;

	stdin_file = NULL;
	stdout_file = NULL;
	stderr_file = NULL;

	/* Check for exit. */
	if (strncmp("exit", line, strlen("exit")) == 0)
		return EXIT_CMD;

	/* Normal command. */
	delim = " \t\n";
	token = strtok_r(line, delim);

	if (token == NULL)
		return ERROR;

	verb = strdup(token);

	/* Copy args. */
	idx = 0;
	while (token != NULL) {
		if (token == NULL) {
			printf(" Expansion failed\n");
			return ERROR;
		}

		args[idx++] = strdup(token);
		token = strtok_r(NULL, delim);
	}

	args[idx++] = NULL;
	return ret;
}

/**
 *  @args - array that contains a simple command with parameters
 */
static void simple_cmd(char **args)
{
	(void)args;
	/**
	 * TODO - Create a process to execute the command.
	 * Use `execvp` to launch the new process.
	 */
}

int main(void)
{
	char line[MAX_LINE_SIZE];

	alloc_mem();

	while (1) {
		printf("> ");
		fflush(stdout);

		memset(line, 0, MAX_LINE_SIZE);

		if (fgets(line, sizeof(line), stdin) == NULL)
			exit(EXIT_SUCCESS);

		type = parse_line(line);

		switch (type) {
		case EXIT_CMD:
			exit(EXIT_SUCCESS);

		case SIMPLE:
			simple_cmd(args);
			break;
		}
	}

	return 0;
}
