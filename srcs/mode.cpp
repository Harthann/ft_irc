#include "mode.hpp"

void	RemoveMode(Commands &cmd, Channel *channel, User *user)
{
	std::string mode = cmd[2];
	for (size_t i = 1; i < mode.length(); ++i)
	{
		if (mode[i] == 'o')
			channel->Privilege(0, user, cmd);
	}
}

void	AddMode(Commands &cmd, Channel *channel, User *user)
{
	std::string mode = cmd[2];
	for (size_t i = 1; i < mode.length(); ++i)
	{
		if (mode[i] == 'o')
			channel->Privilege(1, user, cmd);
	}
}

void	mode_parser(Commands &cmd, Socket *client, Server &server)
{
	User *temp_user = check_user(server.getClients(), client);
	Channel *temp_channel = channel_exist(cmd[1], server);

	if (cmd.length() >= 3)
	{
		if (cmd[2].at(0) != '+')
			RemoveMode(cmd, temp_channel, temp_user);
		else
			AddMode(cmd, temp_channel, temp_user);
	}
}
