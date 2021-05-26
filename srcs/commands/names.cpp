#include "commands_prototypes.hpp"
#include "utils.hpp"

void	list_users_in_chan(std::string chan_name, Socket *client, Server &server) {
	std::string	users_of_channel;
	std::string	response;
	User		*current_user;
	Channel		*chan;

	current_user = check_user(server.getClients(), client);
	if (!(chan = channel_exist(chan_name, server)))
		return ;
	users_of_channel = chan->user_list();
	utils::delete_char(users_of_channel, ',');
	response.append(":" + server.getServerName() + REPLY(RPL_NAMREPLY) + current_user->getNickname() + " @ = " + chan->getName() + " :" + users_of_channel);
	client->bufferize(response);
}

void	list_all(Commands &cmd, Socket *client, Server &server) {
	std::string	users_of_channel;
	std::string	response;
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	for (std::vector<Channel *>::iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it) {
		users_of_channel = (*it)->user_list();
		utils::delete_char(users_of_channel, ',');
		response.append(":" + server.getServerName() + REPLY(RPL_NAMREPLY) + current_user->getNickname() + " = " + (*it)->getName() + " :" + users_of_channel);
		client->bufferize(response);
		response.clear();
		users_of_channel.clear();
	}
	for (std::vector<User *>::iterator it = server.getClients().begin(); it != server.getClients().end(); ++it) {
		if ((*it)->getChannels().empty())
			users_of_channel.append((*it)->getNickname() + " ");
	}
	if (!users_of_channel.empty())
		response.append(":" + server.getServerName() + REPLY(RPL_NAMREPLY) + "* :" + utils::trim(users_of_channel, ' '));
	client->bufferize(response);
}

void	names_command(Commands &cmd, Socket *client, Server &server) {
	std::vector<std::string>	params;

	if (cmd.length() == 1)
		list_all(cmd, client, server);
	else if (cmd.length() == 2 && (utils::split(cmd[1], ',').size()) > 1)
	{
		params = utils::split(cmd[1], ',');
		for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it) {
			utils::trim(*it, ',');
			list_users_in_chan(*it, client, server);
		}
	}
	else
	{
		for (size_t i = 1; i < cmd.length(); ++i)
			list_users_in_chan(cmd[i], client, server);
	}
	client->bufferize(":" + server.getServerName() + REPLY(RPL_ENDOFNAMES) + "* :End of /NAMES list.");
	
}
