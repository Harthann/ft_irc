#include "commands_prototypes.hpp"

void	set_topic(Commands &cmd, Socket *client, Server &server) {
	std::string	topic;
	std::string	response;
	Channel		*chan;
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	if (!(chan = channel_exist(cmd[1], server)))
		return ;
	for (size_t i = 2; i < cmd.length(); ++i)
	{
		if (i == 2 && cmd[i][0] == ':')
			cmd[i].erase(cmd[i].begin());
		topic.append(cmd[i]);
		if (i != cmd.length() - 1)
			topic.append(" ");
	}
	chan->setTopic(topic);
	response.append(current_user->getID() + " TOPIC " + cmd[1] + " :" + topic);
	client->bufferize(response);
}

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
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	if (cmd.length() == 2)
		display_topic(cmd, client, server);
	else if (cmd.length() >= 3)
		set_topic(cmd, client, server);
	else
		client->bufferize(":" + server.getServerName() + " " + utils::itos(ERR_NEEDMOREPARAMS) + " " + current_user->getNickname() + " TOPIC :Not enough parameters");
}
