/*
 * Copyright (c) 2022 Corey Hinshaw
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#define _POSIX_C_SOURCE 199309
#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROGNAME "myterm"
#define CONFIG_FILE "terminal"
#define CONFIG_PATH "/.config"

static bool empty(char *str)
{
	return str == NULL || str[0] == '\0';
}

static char *rtrim(char *str)
{
	int i;
	char *seps = "\t\n\v\f\r ";

	i = strlen(str) - 1;
	while (i >= 0 && strchr(seps, str[i]) != NULL) {
		str[i] = '\0';
		i--;
	}
	return str;
}

static size_t split_args(char *args, char ***argv, size_t size)
{
	size_t argc = size;
	char *arg;

	arg = strtok(args, " ");
	while (arg != NULL) {
		*argv = realloc(*argv, sizeof(char*) * ++argc);
		if (*argv == NULL)
			err(EXIT_FAILURE, NULL);
		(*argv)[argc - 1] = arg;
		arg = strtok(NULL, " ");
	}

	return argc;
}

static char *concat(char *str, unsigned int n, ...)
{
	unsigned int i;
	unsigned int len = 0;
	char *arg;
	va_list ptr, ptr2;

	va_start(ptr, n);
	va_copy(ptr2, ptr);
	for (i = 0; i < n; i++) {
		arg = va_arg(ptr, char*);
		if (arg != NULL) {
			len += strlen(arg);
		}
	}
	va_end(ptr);

	str = realloc(str, len + 1);
	if (str == NULL)
		err(EXIT_FAILURE, NULL);
	str[0] = '\0';

	for (i = 0; i < n; i++) {
		arg = va_arg(ptr2, char*);
		if (arg != NULL) {
			strcat(str, arg);
		}
	}
	va_end(ptr2);

	return str;
}

static int read_term(char *term_cmd, size_t len)
{
	FILE *fp;
	char *path = NULL;
	char *dir;

	dir = getenv("XDG_CONFIG_HOME");
	if (empty(dir)) {
		dir = getenv("HOME");
		if (empty(dir)) {
			warnx("HOME not defined in environment");
			goto error;
		}
		path = concat(path, 4, dir, CONFIG_PATH, "/", CONFIG_FILE);
	} else {
		path = concat(path, 3, dir, "/", CONFIG_FILE);
	}

	if (access(path, F_OK) != 0) {
		return 0;
	}

	if ((fp = fopen(path, "r")) == NULL) {
		dir = getenv("HOME");
		if (empty(dir)) {
			warnx("HOME not defined in environment");
			goto error;
		}
		path = concat(path, 3, dir, "/.", CONFIG_FILE);
		fp = fopen(path, "r");
	}

	if (fp == NULL || fgets(term_cmd, len, fp) == NULL) {
		goto error;
	}
	rtrim(term_cmd);

	free(path);
	return 0;

error:
	if (path)
		free(path);
	return -1;
}

int main (int argc, char **argv)
{
	char *name = PROGNAME;
	char *term = "";
	char term_cmd[PATH_MAX] = "";
	char *strip_arg = "";
	char *term_args = "";
	size_t exec_argc = 0;
	size_t term_argc = 1;
	size_t copy_argc = 0;
	char **exec_argv = NULL;

	if (argc > 0) {
		name = basename(argv[0]);
	}

	term = getenv("TERMINAL");
	strip_arg = getenv("TERMINAL_STRIP");
	term_args = getenv("TERMINAL_ARGS");

	if (empty(term) || strcmp(basename(term), name) == 0) {
		term = getenv("TERMINAL_CMD");
		if (empty(term)) {
			if (read_term(term_cmd, PATH_MAX) != 0) {
				errx(EXIT_FAILURE, "Could not open terminal configuation file!");
			}
			term = term_cmd;
		}
	}
	if (empty(term)) {
		errx(EXIT_FAILURE, "Terminal name not found in environment!");
	}

	if (term_args != NULL) {
		term_argc = split_args(term_args, &exec_argv, 1);
	}

	exec_argc = term_argc + argc - 1;
	if (strip_arg != NULL && strip_arg[0] != '\0') {
		if (argc > 1 && strcmp(strip_arg, argv[1]) == 0) {
			exec_argc--;
		}
	}
	copy_argc = exec_argc - term_argc;

	// exec_argv size: exec_argc + NULL pointer
	exec_argv = realloc(exec_argv, (exec_argc + 1) * sizeof(char *));
	memcpy(exec_argv + term_argc, argv + (argc - copy_argc), (copy_argc + 1) * sizeof(argv));
	exec_argv[0] = term;
	execvp(term, exec_argv);

	err(EXIT_FAILURE, "Error excuting %s", term);
}
