/*-
 * Copyright (c) 2014 Denis Barov
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

#include "dialog4web.h"

const int maxdocsize = 262144;

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

static int
answer_to_connection (
				void *cls,
				struct MHD_Connection *connection,
				const char *url,
				const char *method,
				const char *version,
				const char *upload_data,
				size_t *upload_data_size,
				void **con_cls
				)
{
	
		int ret;
		char *buf = malloc(maxdocsize);
		char *body = malloc(maxdocsize);
		ItemGroupList grouplist;

		InputItem **items;
		struct MHD_Response *response;

		grouplist = get_group_list();

		buf = GroupListStr(grouplist);
		//item = FormItem("ItemName", "This is Item", "checkbox", "id0", "VALUE", "itemclass", "group", true);
		//ret  = InputItemStr(buf, item);
		sprintf(body, bodytemplate, buf);

  response = MHD_create_response_from_buffer (strlen (body), (void *) body,
     MHD_RESPMEM_PERSISTENT);
  MHD_add_response_header (response, "Content-Type", MIMETYPE);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
	free(body);
	free(buf);
  return ret;
}

int
main ()
{
  struct MHD_Daemon *daemon;

  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == daemon)
    return 1;

  getchar ();

  MHD_stop_daemon (daemon);
  return 0;
}
