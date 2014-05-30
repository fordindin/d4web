#include "dialog4web.h"

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

int get_items(InputItem **items, StringList *itemnames, char *type, char *groupname){
		const char *desc;
		*items = (InputItem *)calloc(itemnames->sl_cur, sizeof(InputItem));
		for (int i=0; i < itemnames->sl_cur; i++){
				desc = get_desc(itemnames->sl_str[i], "");
				FormItem (&items[i],
						itemnames->sl_str[i],
						desc,
						type,
						itemnames->sl_str[i],
						itemnames->sl_str[i],
						type,
						groupname,
						true
						// XXX: checked is temporary parameter
						);
		}
		return 0;
		
}

int env_get_group(ItemGroup *itemgroup, char *name, char *type){
		char buf[256];
		const char *desc;
		StringList *itemnames;
		InputItem *items;

		if (snprintf(buf, sizeof(buf), "OPTIONS_%s_%s", type, name) >= (int)sizeof(buf))
				warnx("group list %s has been truncated", name);
		desc = get_desc(name, "");
		itemnames = parse_env_sl(buf);
		get_items(&items, itemnames, type, name);
		itemgroup = &(struct ItemGroup){
				.name = strdup(name),
				.desc = strdup(desc),
				.items = items
		};
		return 0;
}

int main(){
	ItemGroup test;
	//test = malloc(sizeof(ItemGroup));
	env_get_group(&test, "M1", "MULTI");
	printf("%s\n", test.name);
}
