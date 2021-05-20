#include "Server.hpp"
#include "ft_irc.hpp"
#include "Channel_Registration.hpp"
#include "numeric_replies.hpp"

void	messages_command(Commands &cmd, Socket *client, Server &server)
{
	void	*ptr;

	/*		To add check if user has right to send to channel either way send 404 error */
	cmd.setFrom(check_user(server.getClients(), client)->getID());
	if (cmd.length() != 3)
	{
		if (cmd.length() > 3)
			client->bufferize(":" + server.getServerName() + " " + ERR_TOOMANYTARGETS + ":Too many target");
		else if (cmd.length() > 1 && cmd[1][0] == ':')
			client->bufferize(":" + server.getServerName() + " " + ERR_NORECIPIENT + ":No recipient given (PRIVMSG)");
		else
			client->bufferize(":" + server.getServerName() + " " + ERR_NOTEXTTOSEND + ":No text to send");
	}
	else if ((ptr = channel_exist(cmd[1], server)))
	{
		reinterpret_cast<Channel*>(ptr)->SendMsgToAll(cmd.as_string(), check_user(server.getClients(), client));
	}
	else if ((ptr = server.getUserByName(cmd[1])))
	{
		reinterpret_cast<User*>(ptr)->getSocketPtr()->bufferize(cmd);
	}
	else
		client->bufferize(":" + server.getServerName() + " " + ERR_NOSUCHNICK + cmd[1] + " :No such nick/channel");
}
