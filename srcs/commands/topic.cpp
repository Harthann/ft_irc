#include "commands_prototypes.hpp"
void	clear_topic(Commands &cmd, Socket *client, Server &server) {
	std::string	response;
	User		*current_user;
	Channel		*chan;

	current_user = check_user(server.getClients(), client);
	if (!(chan = channel_exist(cmd[1], server)))
		response.append(cmd[1] + ":No such channel");
	else
	{
		chan->setTopic(std::string());
		response.append(":" + chan->getServerName() + REPLY(RPL_NOTOPIC) + current_user->getNickname() + " " + chan->getName() + " :No topic is set");
	}
	client->bufferize(response);
}

void	set_topic(Commands &cmd, Socket *client, Server &server) {
	std::string	topic;
	std::string	response;
	Channel		*chan;
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	if (!(chan = channel_exist(cmd[1], server)))
		response.append(cmd[1] + ":No such channel");
	else if (checking_a_bit(chan->getMode(),TOPIC_FLAG)  && !chan->CheckIfChannelOperator(current_user))
	{
		response.append(":" + server.getServerName()
							+ REPLY(ERR_CHANOPRIVSNEEDED)
							+ current_user->getNickname() + " "
							+ chan->getName()
							+ " :You are not channel operator");
	}
	else
	{
		topic.append(cmd[2]);
		chan->setTopic(topic);
		response.append(current_user->getID() + " TOPIC " + cmd[1] + " :" + topic);
	}
	client->bufferize(response);
}

void	display_topic(Commands &cmd, Socket *client, Server &server) {
	Channel		*chan;
	std::string	response;
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	chan = channel_exist(cmd[1], server);
	if (!chan)
		response.append(cmd[1] + ":No such channel");
	else
	{
		if (!(chan->getTopic().empty()))
			response.append(":" + chan->getServerName() + REPLY(RPL_TOPIC) + current_user->getNickname() + " " + chan->getName() + " :" + chan->getTopic());
		else
			response.append(":" + chan->getServerName() + REPLY(RPL_NOTOPIC) + current_user->getNickname() + " " + chan->getName() + " :No topic is set.");
	}
	client->bufferize(response);
}

void	topic_command(Commands &cmd, Socket *client, Server &server) {
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	if (cmd.length() == 2)
		display_topic(cmd, client, server);
	else if (cmd.length() == 3 && cmd[2].length() == 1 && cmd[2][0] == ':')
		clear_topic(cmd, client, server);
	else if (cmd.length() >= 3)
		set_topic(cmd, client, server);
	else
		client->bufferize(":" + server.getServerName() + REPLY(ERR_NEEDMOREPARAMS) + current_user->getNickname() + " TOPIC :Not enough parameters");
}
