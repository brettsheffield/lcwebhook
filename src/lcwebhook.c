#define _GNU_SOURCE
#include <librecast.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
