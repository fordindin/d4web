#include <stdio.h>

#include "dialog4web.h"

int FormItem (InputItem **item,
		const char *name, const char *desc, const char *type,
		const char *id, const char *value, const char *hclass,
		const char *group, bool checked
		){
		*item = &(struct InputItem){
				.name = name,
				.desc = desc,
				.type = strdup(type),
				.id = strdup(id),
				.value = strdup(value),
				.hclass = strdup(hclass),
				.group = strdup(group),
				.checked = checked
		};
		/*
		item->name = malloc(sizeof(name));
		item->name = strdup(name);
		item->desc = malloc(sizeof(desc));
		item->desc = strdup(desc);
		item->type = strdup(type);
		item->id = strdup(id);
		item->value = strdup(value);
		item->hclass = strdup(hclass);
		item->group = strdup(group);
		item->checked = checked;
		*/
		return 0;
}

int InputItemStr( char *out, InputItem *item ){
		sprintf(out,"<span>%s<input name='%s' type='%s' id='%s' value='%s' class='%s' group='%s' %s />\n",
						item->desc, item->name, item->type,
						item->id, item->value, item->hclass, item->group,
						item->checked ? "checked" : "" );
		return 0;
}

int DocumentBody (char *out){
		sprintf(out, "<html><body>%%s</body></html>");
		return 0;
}
/*
int
main(){
		char *out;
		int ret;
		InputItem *item;
		item = FormItem("ItemName", "This is Item", "checkbox", "id0", "VALUE", "itemclass", 0);
		ret  = InputItemStr(out, item);
		printf ("%s\n", out);
}
*/
