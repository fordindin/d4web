
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <microhttpd.h>
#include <stdio.h>
#include <err.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stringlist.h>

#define PORT 8888
#define MIMETYPE "text/html"
#define MAXDOCSIZE 1048576

#define ITEM_CHECK      1
#define ITEM_RADIO      2

typedef struct InputItem {
		const char *name;
		const char *desc;
		const char *type;
		const char *id;
		const char *value;
		bool isnew;
		const char *group;
		bool checked;
} InputItem;

typedef InputItem * ItemList;

typedef struct ItemGroup {
		const char *name;
		const char *desc;
		InputItem *items;
		int nitems;
} ItemGroup;

int InputItemStr( char *out, InputItem *item );
int DocumentBody( char *out);

