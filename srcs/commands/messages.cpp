#include "Server.hpp"
#include "ft_irc.hpp"
#include "Channel_Registration.hpp"

void	messages_command(Commands &cmd, Socket *client, Server &server)
{
	void	*ptr;

	if ((ptr = channel_exist(cmd[1], server)))
	{
		std::cout << "Message to channel" << std::endl;
		cmd.setFrom(":" + check_user(server.getClients(), client)->getNickname());
		reinterpret_cast<Channel*>(ptr)->SendMsgToAll(cmd.as_string(), check_user(server.getClients(), client));
	}
	else if (server.getUserByName(cmd[1]))
	{
		std::cout << "Message to user" << std::endl;
	}
	else
		std::cout << "Couldn't find user or channel" << std::endl;
}
