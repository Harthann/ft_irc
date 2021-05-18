#include "commands_prototypes.hpp"

void	display_topic(Commands &cmd, Socket *client, Server &server) {
	Channel		*chan;
	std::string	response;
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	chan = channel_exist(cmd[1], server);
	if (!chan)
		response.append(chan->getName() + ":No such channel");
	else
	{
		if (!(chan->getTopic().empty()))
			response.append(":" + chan->getServerName() + " " + utils::itos(RPL_TOPIC) + " " + current_user->getNickname() + " " + chan->getName() + " :" + chan->getTopic());
		else
			response.append(":" + chan->getServerName() + " " + utils::itos(RPL_NOTOPIC) + " " + current_user->getNickname() + " " + chan->getName() + " :No topic is set.");
	}
	client->bufferize(response);
}

void	topic_command(Commands &cmd, Socket *client, Server &server) {
	if (cmd.length() == 2)
		display_topic(cmd, client, server);
//	else if (cmd.length() == 3)
//		set_topic(cmd, client, server);
}
