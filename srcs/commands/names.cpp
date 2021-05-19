#include "commands_prototypes.hpp"
#include "utils.hpp"

void	list_all(Commands &cmd, Socket *client, Server &server) {
	std::string				users_of_channel;
	std::string				response;
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	for (std::vector<Channel *>::iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it) {
		users_of_channel = (*it)->user_list();
		utils::delete_char(users_of_channel, ',');
		response.append(":" + server.getServerName() + " " + RPL_NAMREPLY + " " + current_user->getNickname() + " = " + (*it)->getName() + " :" + users_of_channel);
		client->bufferize(response);
		response.clear();
	}
	response.append(":" + server.getServerName() + " " + RPL_ENDOFNAMES + " * :End of /NAMES list.");
	client->bufferize(response);
}

void	names_command(Commands &cmd, Socket *client, Server &server) {
	if (cmd.length() == 1)
		list_all(cmd, client, server);
