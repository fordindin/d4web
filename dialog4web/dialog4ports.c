/*-
 * Copyright (c) 2013 Ilya A. Arkhipov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#include <err.h>
#include <errno.h>
#include <dialog.h>
#include <dlg_keys.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stringlist.h>
#include <locale.h>
#include <unistd.h>

#include "mixedlist.h"

#define	D4PVERSION	"0.1.5"

static int list_no = 0;
static int group = 0;

/* First items */
static int items_sz = 5;
/* Enables items */
static StringList *enable_items = NULL;
/* New items */
static StringList *new_items = NULL;

/* add item to items */
static void
add_item(dialog_mixedlist **items, char const *name, char const *text,
		bool state, bool new, int type, int grp)
{

	if ((list_no + 1 > items_sz) || *items == NULL) {
		items_sz *= 2;
		*items = realloc(*items, items_sz * sizeof(dialog_mixedlist));
		if (*items == NULL)
			err(EXIT_FAILURE, "Need more memory!");
	}

	/* XXX: add more checks */
	(*items)[list_no].name = strdup(name);
	(*items)[list_no].text = strdup(text);
	(*items)[list_no].state = state;
	(*items)[list_no].type = type;
	(*items)[list_no].group = grp;
	(*items)[list_no].new = new;
	list_no++;
}

/* get description in env: %s_DESC */
static char const *
get_desc(char const *res, char const *fallback)
{
	char buf[256];
	char *desc;

	if (snprintf(buf, sizeof(buf), "%s_DESC", res) >= (int)sizeof(buf))
		warnx("Description for %s has been truncated", res);

	desc = getenv(buf);
	if (desc == NULL || desc[0] == '\0')
		return (fallback);

	return (desc);
}

static bool
is_enable(char *name)
{
	return (sl_find(enable_items, name) != NULL);
}

static bool
is_new(char *name)
{
	return (sl_find(new_items, name) != NULL);
}

static StringList *
parse_env_sl(char const *env_name)
{
	StringList *sl;
	char *env;
	char *temp;
	char *token;

	sl = sl_init();
	env = getenv(env_name);
	if (env != NULL) {
		temp = strdup(env);
		while ((token = strsep(&temp, " \t")) != NULL) {
			if (token[0] == '\0')
				continue;
			sl_add(sl, strdup(token));
		}
		free(temp);
	}
	return (sl);
}

/* parsing part */
static int
parsing_env(dialog_mixedlist **items, char const *env_name, int type)
{
	char *env, buf[256];
	char const *delimiter = " \t";
	char *token, *token2;
	char *temp, *tofree;
	char *temp2, *tofree2;

	env = getenv(env_name);
	if (env == NULL)
		return (0);

	if (strcmp(env_name, "ALL_OPTIONS") == 0) {
		tofree = temp = strdup(env);

		while ((token = strsep(&temp, delimiter)) != NULL) {
			if (token[0] == '\0')
				continue;
			add_item(items, token, get_desc(token, ""), is_enable(token),
					is_new(token), type, group);
		}
		free(tofree);
	} else {
		tofree = temp = strdup(env);
		while ((token = strsep(&temp, delimiter)) != NULL) {
			if (token[0] == '\0')
				continue;
			if (list_no != 0)
				add_item(items, get_desc(token, token), "", false, false,
						ITEM_SEPARATOR, group);

			snprintf(buf, sizeof(buf), "%s_%s", env_name, token);
			env = getenv(buf);
			if (env == NULL)
				errx(EXIT_FAILURE, "%s does not exists", buf);
			tofree2 = temp2 = strdup(env);
			while ((token2 = strsep(&temp2, delimiter)) != NULL) {
				if (token2[0] == '\0')
					continue;
				add_item(items, token2, get_desc(token2, ""),
						is_enable(token2), is_new(token2), type, group);
			}
			free(tofree2);
			group++;
		}

		free(tofree);
	}
	if (group == 0)
		group++;

	return (0);
}


/* prepare items for next drawing*/
static dialog_mixedlist *
prepare_items(void)
{
	dialog_mixedlist *items = NULL;

	enable_items = parse_env_sl("PORT_OPTIONS");
	new_items = parse_env_sl("NEW_OPTIONS");

	parsing_env(&items, "ALL_OPTIONS", ITEM_CHECK);
	parsing_env(&items, "OPTIONS_GROUP", ITEM_CHECK);
	parsing_env(&items, "OPTIONS_MULTI", ITEM_CHECK);
	parsing_env(&items, "OPTIONS_SINGLE", ITEM_RADIO);
	parsing_env(&items, "OPTIONS_RADIO", ITEM_RADIO);

	return (items);
}

static int
mixedlist_show(const char *title, const char *cprompt, int height,
		int min_height, int width, dialog_mixedlist *items, int align_center)
{
	int i = 0;
	int res;

	if (list_no == 0) {
		end_dialog();
		err(EXIT_FAILURE, "List of items should not be empty");
	} else
		res = dlg_mixedlist(title, cprompt, height, min_height, width,
				list_no, items, &i, align_center);

	return (res);
}

static void
usage(void)
{
	fprintf(stderr,"Usage: dialog4ports [-hv]\n");
	fprintf(stderr,"For more information please read man dialog4ports.\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	char *portname, *temp;
	char buf[256];
	int i, res, ch;
	int align_center = 0;
	int height = 0, width = 80; // default value
	int min_height = 0;
	char *helpfile;
	dialog_mixedlist* items;

	setlocale(LC_ALL, "");
	errno = 0;

	while ((ch = getopt(argc, argv, "hv?")) != -1)
		switch (ch) {
			case 'v':
				fprintf(stderr,"dialog4ports version: %s\n", D4PVERSION);
				exit(0);
				break;
			case '?':
			case 'h':
				usage();
				break;
			default:
				exit(1);
		}
	argc -= optind;
	argv += optind;

	init_dialog(stdin, stdout);

	temp = getenv("D4PHEIGHT");
	if (temp != NULL) {
		errno = 0;
		height = strtol(temp, NULL, 0);
		if (errno != 0)
			height = 0;
	}

	temp = getenv("D4PMINHEIGHT");
	if (temp != NULL && height == 0) {
		errno = 0;
		/*
		 * 6 it is magic number, it include all borders
		 * buttons space etc.
		 * */
		min_height = strtol(temp, NULL, 0) + 6;
		if (errno != 0)
			min_height = 0;
	}

	temp = getenv("D4PWIDTH");
	if (temp != NULL) {
		errno = 0;
		width = strtol(temp, NULL, 0);
		if (errno != 0)
			width = 80;
	}

	temp = getenv("D4PFULLSCREEN");
	if (temp != NULL &&
		(strcasecmp(temp, "Y") == 0 || strcasecmp(temp, "YES") == 0)) {
			height = SLINES - 4;
			width = SCOLS - 4;
	}

	temp = getenv("D4PALIGNCENTER");
	if (temp != NULL &&
		(strcasecmp(temp, "Y") == 0 || strcasecmp(temp, "YES") == 0))
			align_center = 1;

	portname = getenv("PKGNAME");
	if (portname == NULL) {
		end_dialog();
		fprintf(stderr, "env PKGNAME is NULL\n");
		usage();
	}

	if ((helpfile = getenv("PKGHELP")) != NULL) {
		if (eaccess(helpfile, R_OK) != 0) {
			end_dialog();
			err(EXIT_FAILURE, "%s", helpfile);
		}

		dialog_vars.help_file = helpfile;
	}

	items = prepare_items();

	snprintf(buf, sizeof(buf), " %s ", portname);

	/*	For future using
	 *	dialog_vars.extra_button = TRUE;
	 *	dialog_vars.extra_label = __DECONST(char *, "Port license");
	 */

	dlg_clear();
	res = mixedlist_show(buf, "", height, min_height, width, items, align_center);

	if (res == 0) {
		/* return all active items */
		for (i = 0; i < list_no; i++) {
			if (items[i].state == 1) {
				fprintf(stderr, "\"%s\"", items[i].name);
				fprintf(stderr, " ");
			}
		}
	} else {
		return (1);
	}

	return (0);
}
