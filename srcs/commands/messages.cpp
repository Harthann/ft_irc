#include "Server.hpp"
#include "ft_irc.hpp"
#include "Channel_Registration.hpp"
#include "numeric_replies.hpp"


static void	channel_messages(Commands cmd, Socket *client, Server &server, Channel *channel, std::string &target)
{
	server.logString("Channel message received with target : " + target);
	if (checking_a_bit(channel->getMode(), MESSAGE_FLAG) && !channel->getUserByName(check_user(server.getClients(), client)->getNickname()))
	{
		client->bufferize(":" + server.getServerName() + REPLY(ERR_CANNOTSENDTOCHAN) + channel->getName() + " :Cannot send to nick/channel");
		return ;
	}
	cmd[1] = target;
	channel->SendMsgToAll(cmd.as_string(), check_user(server.getClients(), client));
}

static void	user_messages(Commands cmd, Socket *client, Server &server, User *user, std::string &target)
{
	if (user->getAwayMessage().empty())
	{
		cmd[1] = target;
		user->getSocketPtr()->bufferize(cmd);
	}
	else
		client->bufferize(":" + server.getServerName() + REPLY(RPL_AWAY)
							+ check_user(server.getClients(), client)->getNickname() + " "
							+ user->getNickname() + " :" + user->getAwayMessage());
}

/* nice spaghetti code */

void	extract_targets(std::string &list, std::vector<std::string> &target_list)
{
	size_t	pos1 = 0;
	size_t	pos2 = 0;

	do
	{
		pos2 = list.find(',', pos1);
		if (pos2 == std::string::npos)
			pos2 = list.length();
		target_list.push_back(list.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
	} while (pos2 != std::string::npos && pos2 != list.length());
}

void	messages_command(Commands &cmd, Socket *client, Server &server)
{
	void						*ptr;
	std::vector<std::string>	targets;

	/*		To add check if user has right to send to channel either way send 404 error */
	cmd.setFrom(check_user(server.getClients(), client)->getID());
	if (cmd.length() != 3)
	{
		if (cmd.length() > 3)
			client->bufferize(":" + server.getServerName() + REPLY(ERR_TOOMANYTARGETS) + ":Too many target");
		else if (cmd.length() > 1 && cmd[1][0] == ':')
			client->bufferize(":" + server.getServerName() + REPLY(ERR_NORECIPIENT) + ":No recipient given (PRIVMSG)");
		else
			client->bufferize(":" + server.getServerName() + REPLY(ERR_NOTEXTTOSEND) + ":No text to send");
	}
	else
	{
		extract_targets(cmd[1], targets);
		for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
		{
			if ((ptr = channel_exist(*it, server)))
				channel_messages(cmd, client, server, reinterpret_cast<Channel*>(ptr), *it);
			else if ((ptr = server.getUserByName(*it)))
				user_messages(cmd, client, server, reinterpret_cast<User*>(ptr), *it);
			else
				client->bufferize(":" + server.getServerName() + REPLY(ERR_NOSUCHNICK) + *it + " :No such nick/channel");
		}
	}
}

void	notice_command(Commands &cmd, Socket *client, Server &server)
{
	cmd.setFrom(check_user(server.getClients(), client)->getID());
	if (cmd.length() == 3 && server.getUserByName(cmd[1]))
		server.getUserByName(cmd[1])->getSocketPtr()->bufferize(cmd);
}

void	away_command(Commands &cmd, Socket *client, Server &server)
{
	User		*tmp;

	tmp = check_user(server.getClients(), client);
	if (cmd.length() == 1 && tmp)
	{
		tmp->setAwayMessage("");
		tmp->getSocketPtr()->bufferize(":" + server.getServerName() + REPLY(RPL_UNAWAY) + tmp->getNickname() + ":You are no longer marked as being away");
	}
	else if (tmp)
	{
		tmp->setAwayMessage(cmd[1].substr(1, cmd[1].length()));
		std::cout << "Away message sent to : " << tmp->getAwayMessage() << std::endl;
		tmp->getSocketPtr()->bufferize(":" + server.getServerName() + REPLY(RPL_NOWAWAY) + tmp->getNickname() + ":You have been marked as being away");
	}
}
