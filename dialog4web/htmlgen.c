#include <stdio.h>

#include "dialog4web.h"

int InputItemStr( char *out, InputItem *item ){
		sprintf(out,"<span>%s<input name='%s' type='%s' id='%s' value='%s' class='%s' %s />\n",
						item->desc, item->group, item->type,
						item->id, item->value, item->isnew ? "new" : "old",
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
