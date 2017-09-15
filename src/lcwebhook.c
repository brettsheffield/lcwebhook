/*
 * lcwebhook.c - some code to handle incoming connections
 *
 * this file is part of LCWEBHOOK
 *
 * Copyright (c) 2017 Brett Sheffield <brett@gladserv.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING in the distribution).
 * If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE
#include <librecast.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define BUFSIZE 5242880 /* github has a 5MB limit on webhook payloads */

int main(int argc, char **argv)
{
	ssize_t len;
	char buf[BUFSIZE];
	char *channelName;
	char *msgtext = NULL;

	/* read from stdin */
	len = fread(buf, 1, BUFSIZE, stdin);
	fprintf(stdout, "Hey, thanks for the %i bytes!\n", (int)len);

	/* parse JSON */
	cJSON *root = cJSON_Parse(buf);
	if (root == NULL)
		return 1; /* failed to parse */
	cJSON *zen = cJSON_GetObjectItemCaseSensitive(root, "zen");
	if (zen != NULL)
		asprintf(&msgtext, "<github> '%s' - I think there's something in that for all of us, don't you?\n", zen->valuestring);
	cJSON_Delete(root);

	if (msgtext == NULL)
		return 1;

	if (argc > 1)
		asprintf(&channelName, "%s", argv[1]);
	else
		asprintf(&channelName, "#chatx");

	/* send to librecast channel */
	lc_ctx_t *ctx;
	lc_socket_t *sock;
	lc_channel_t *chan;

	ctx = lc_ctx_new();
	sock = lc_socket_new(ctx);
	chan = lc_channel_new(ctx, channelName);
	lc_channel_bind(sock, chan);

	lc_message_t msg;
	lc_msg_init_size(&msg, strlen(msgtext) - 1);
	memcpy(lc_msg_data(&msg), msgtext, strlen(msgtext) - 1);
	lc_msg_send(chan, &msg);

	/* clean up */
	lc_socket_close(sock);
	lc_channel_free(chan);
	lc_ctx_free(ctx);
	free(channelName);
	free(msgtext);

	return 0;
}
