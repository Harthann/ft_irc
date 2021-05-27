#include "Server.hpp"
#include "ft_irc.hpp"
#include "Channel_Registration.hpp"
#include "numeric_replies.hpp"


static void	channel_messages(Commands &cmd, Socket *client, Server &server, Channel *channel)
{
	channel->SendMsgToAll(cmd.as_string(), check_user(server.getClients(), client));
}

static void	user_messages(Commands &cmd, Socket *client, Server &server, User *user)
{
	if (user->getAwayMessage().empty())
			user->getSocketPtr()->bufferize(cmd);
	else
		client->bufferize(":" + server.getServerName() + REPLY(RPL_AWAY)
							+ check_user(server.getClients(), client)->getNickname() + " "
							+ user->getNickname() + " :" + user->getAwayMessage());
}

void	messages_command(Commands &cmd, Socket *client, Server &server)
{
	void	*ptr;

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
	else if ((ptr = channel_exist(cmd[1], server)))
		channel_messages(cmd, client, server, reinterpret_cast<Channel*>(ptr));
	else if ((ptr = server.getUserByName(cmd[1])))
		user_messages(cmd, client, server, reinterpret_cast<User*>(ptr));
	else
		client->bufferize(":" + server.getServerName() + REPLY(ERR_NOSUCHNICK) + cmd[1] + " :No such nick/channel");
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
