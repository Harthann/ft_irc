#include "commands_prototypes.hpp"
#include "utils.hpp"

void	kill_by_server(Commands &cmd, Server &server, std::string msg) {
	User	*user;

	for (Server::user_vector::iterator it = server.getClients().begin(); it != server.getClients().end(); ++it) {
		if ((*it)->getNickname() == cmd[1])
		{
			user = (*it);
			user->partChannels();
			server.delete_user(user, msg);
			server.addUnavailableNickname(cmd[1]);
			delete user;
		}
	}
}
