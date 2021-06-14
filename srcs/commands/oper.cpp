#include "ft_irc.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Commands.hpp"
#include "Channel_Registration.hpp"

void	oper_commands(Commands &cmd, Socket *client, Server &server)
{
	User *tmp;

	if (cmd.length() != 3)
		return ;
	if (cmd[2] == server.getPassword())
	{
		tmp = server.getUserByName(cmd[1]);
		if (!tmp)
		{

		}
		else
		{
			tmp->enableFlag(OPERATOR_FLAG);
			server.logString("User : " + tmp->getNickname() + " has been promot has server operator");
			client->bufferize(":" + server.getServerName() + " MODE " + tmp->getNickname() + " :+o");
		}
	}
}
