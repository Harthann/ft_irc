#include "Channel_Registration.hpp"

User	*check_user(std::vector<User *>server_users, Socket *client)
{
	User *temp;
	temp = NULL;
	for (unsigned long i = 0; i < server_users.size(); i++)
	{
		if(server_users[i]->getSocketPtr() == client)
			temp = server_users[i];
	}
	return temp;
}

Channel		*channel_exist(std::string name, Server &server)
{
	for (unsigned long i = 0; i < server.getChannels().size(); i++)
	{
		if(server.getChannels()[i]->getName() == name)
			return server.getChannels()[i];
	}
	return NULL;
}

void	add_member(User *user, Server &server, std::string name)
{
	for (unsigned long i = 0; i < server.getChannels().size(); i++)
	{
		if(server.getChannels()[i]->getName() == name)
		{
			server.getChannels()[i]->addUser(user);
			user->ActiveChannel(server.getChannels()[i]);
		}
	}
}

int		CheckChannelName(std::string name)
{
	if (name[0] == '#' || name[0] == '&' || name[0] == '+' || name[0] == '!')
	{
		for (unsigned int i = 0; i < name.length(); ++i)
		{
			if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
				return 0;
		}
		return 1;
	}
	return 0;
}

void	add_to_channel(Commands cmd, Socket *client, Server &server)
{
	User *current_user;
	Channel *res;

	current_user = check_user(server.getClients(), client);
	if(!channel_exist(cmd[1], server))
	{
		if (CheckChannelName(cmd[1]))
		{
		res = new Channel(cmd[1], current_user, server.getServerName());
		server.addChannel(res);
		current_user->ActiveChannel(res);
		}
	}
	else
		add_member(current_user, server, cmd[1]);
	cmd.setFrom(current_user->getNickname());
}

void	part_from_channel(Commands cmd, Socket *client, Server &server)
{
	User *current_user;
	current_user = check_user(server.getClients(), client);
	if(channel_exist(cmd[1], server))
		current_user->partChannel(cmd[1]);

}
