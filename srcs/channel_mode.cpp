#include "mode.hpp"

void	mode_channel(Commands &cmd, Socket *client, Server &server) {
	User	*temp_user = check_user(server.getClients(), client);
	Channel	*temp_channel = channel_exist(cmd[1], server);

	if (cmd.length() >= 3)
			mode_add_or_remove(cmd, temp_channel, temp_user);
}
