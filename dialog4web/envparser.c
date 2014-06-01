#include "dialog4web.h"

static StringList *enable_items = NULL;
static StringList *new_items = NULL;

static bool
is_enable(const char *name)
{
	return (sl_find(enable_items, (char *)name) != NULL);
}

static bool
is_new(const char *name)
{
	return (sl_find(new_items, (char *)name) != NULL);
}


const char *
type_map(const char *input){
		if (strcmp(input, "GROUP") == 0)
				return "checkbox";
		if (strcmp(input, "MULTI") == 0)
				return "checkbox";
		if (strcmp(input, "SINGLE") == 0)
				return "radio";
		if (strcmp(input, "RADIO") == 0)
				return "radio";
		if (strcmp(input, "BUTTON") == 0)
				return "button";
		return "button";
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

int get_items(ItemList *items, StringList *itemnames, char *type, char *groupname){
		const char *desc, *name;
		int sl_len, nitems;
		nitems = 0;
		sl_len = itemnames->sl_cur;
		*items = (ItemList)calloc(sl_len, sizeof(InputItem));

		for (int i=0; i < sl_len; i++){
				name = strdup(itemnames->sl_str[i]);
				desc = get_desc(itemnames->sl_str[i], "");
				items[i] = (InputItem *)malloc(sizeof(InputItem));
				(*items)[i].name = strdup(name);
				(*items)[i].desc = strdup(desc);
				(*items)[i].type = strdup(type);
				(*items)[i].id = strdup(name);
				(*items)[i].value = strdup(name);
				(*items)[i].isnew = is_new(name);
				(*items)[i].group = strdup(groupname);
				(*items)[i].checked = is_enable(name);
				nitems++;
		}
		return nitems;
		
}

ItemGroup *
env_get_group(char *name, char *type){
		char buf[256];
		const char *desc;
		StringList *itemnames = NULL;
		ItemGroup *itemgroup;
		ItemList items;
		int nitems;
		itemgroup = (ItemGroup *)malloc(sizeof(ItemGroup));

		/* there is special case for ALL_OPTIONS: it is a checkbox group without
		 * description and with non-standart option list 
		 */
		if (strcmp(name, "ALL") == 0){
				sprintf(buf,"ALL_OPTIONS");
				desc = "All options";
		}
		else {
				if (snprintf(buf, sizeof(buf), "OPTIONS_%s_%s", type, name) >= (int)sizeof(buf))
						warnx("group list %s has been truncated", name);
				desc = get_desc(name, "");
		}
		itemnames = parse_env_sl(buf);
		nitems = get_items(&items, itemnames, type, name);
		(*itemgroup).name = strdup(name);
		(*itemgroup).desc = strdup(desc);
		(*itemgroup).items = (ItemList)items;
		(*itemgroup).nitems = nitems;
		return (itemgroup);
}

int main(){
	enable_items = parse_env_sl("PORT_OPTIONS");
	new_items = parse_env_sl("NEW_OPTIONS");
	ItemGroup *test;


	test = env_get_group("M2", "MULTI");
	//env_get_group(&test, "ALL", "MULTI");
	printf("%d\n", test->nitems);
	printf("%s\n", test->items[0].name);
}
