#include "server.h"

void srv_free_client(client_t *clt) {
	close(clt->fd);
	buffer_free(&clt->buffer);
	if (!clt->room_name[0])
		return ;
	room_info_t *room = oe_hashmap_get(&srv->rooms, clt->room_name);
	if (!room)
		return ;
	for (int i = 0; room->players[i]; i++) {
		if (!strcmp(room->players[i], clt->name)) {
			free(room->players[i]);
			for (int k = i; room->players[k]; k++)
				room->players[k] = room->players[k + 1];
			room->players = realloc(room->players, sizeof(char *) * (1 + oe_strarr_len(room->players)));
			break;
		}
	}
	printf("players %s\n", room->players[0]);
	srv->room_libs[room->type].leave(room, clt);
	if (!room->players[0]) {
		srv_free_room(room);
		oe_hashmap_remove(&srv->rooms, room->name, NULL);
		free(room);
	}
}

void srv_free_room(room_info_t *room) {
	for (int i = 0; room->players && room->players[i]; i++)
		free(room->players[i]);
	free(room->players);
	srv->room_libs[room->type].free(room);
}
