#include "commands_prototypes.hpp"
#include "utils.hpp"

void		version_command(Commands &cmd, Socket *client, Server &server) {
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	client->bufferize(":" + server.getServerName() + RPL_VERSION + current_user->getNickname() + " 1.0.0 " + server.getServerName());
}
