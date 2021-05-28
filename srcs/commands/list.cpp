#include "commands_prototypes.hpp"
#include "utils.hpp"

void	list_all_topics(Commands &cmd, Socket *client, Server &server) {
	std::string	response;
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	for (std::vector<Channel *>::iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it) {
		if ((!(*it)->IsSecret()) || ((*it)->IsSecret() && (*it)->getUserByName(current_user->getNickname()))) {
			response.append(":" + server.getServerName() + REPLY(RPL_LIST) + current_user->getNickname() + " " + (*it)->getName());
			// if ((*it)->IsPrivate())
			// 	response.append(" Prv");
			response.append(std::string(" 1") + " :" + (*it)->getTopic());
			client->bufferize(response);
			response.clear();
		}
	}
}

void	list_channels_topics(std::string chan_name, Socket *client, Server &server) {
	std::string	channel_topic;
	std::string	response;
	User		*current_user;
	Channel		*chan;

	current_user = check_user(server.getClients(), client);
	if (!(chan = channel_exist(chan_name, server)))
		return ;
	channel_topic = chan->getTopic();
	response.append(":" + server.getServerName() + REPLY(RPL_LIST) + current_user->getNickname() + " " + chan->getName() + " 1" + " :" + channel_topic);
	client->bufferize(response);
}

void	list_command(Commands &cmd, Socket *client, Server &server) {
	std::string					response;
	std::vector<std::string>	params;
	User						*current_user;

	current_user = check_user(server.getClients(), client);
	if (cmd.length() == 1)
		list_all_topics(cmd, client, server);
	else if (cmd.length() == 2 && (utils::split(cmd[1], ',').size()) > 1) {
		params = utils::split(cmd[1], ',');
		for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it) {
			utils::trim(*it, ',');
			list_channels_topics(*it, client, server);
		}
	}
	else if (cmd.length() == 2)
		list_channels_topics(cmd[1], client, server);
	response.append(":" + server.getServerName() + REPLY(RPL_LISTEND) + current_user->getNickname() + " :End of LIST");
	client->bufferize(response);
}
