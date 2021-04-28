#include "Channel_Registration.hpp"

User	 check_user(std::vector<User> &temp_users, std::vector<User>server_users, Socket *client)
{
	User temp;
	for (unsigned long i = 0; i < temp_users.size(); i++)
	{
		if(temp_users[i].getSocketPtr() == client)
			temp = temp_users[i];
	}
	for (unsigned long i = 0; i < server_users.size(); i++)
	{
		if(server_users[i].getSocketPtr() == client)
			temp = server_users[i];
	}
	return temp;
}

int		channel_exist(std::string name, Server &server)
{
	for (unsigned long i = 0; i < server.getChannels().size(); i++)
	{
		if(server.getChannels()[i].getName() == name)
			return 1;
	}
	return 0;
}

void	add_member(User &user, Server &server, std::string name)
{
	for (unsigned long i = 0; i < server.getChannels().size(); i++)
	{
		if(server.getChannels()[i].getName() == name)
		{
			server.getChannels()[i].addUser(user);
			user.ActiveChannel(&server.getChannels()[i]);
		}
	}
}

void	add_to_channel(Commands cmd, Socket *client, Server &server, std::vector<User> &temp_users)
{
	User current_user;

	current_user = check_user(temp_users, server.getClients(), client);
	if(!channel_exist(cmd[1], server))
	{
		Channel temp(cmd[1], current_user);
		server.addChannel(temp);
		current_user.ActiveChannel(&temp);
	}
	else
		add_member(current_user, server, cmd[1]);
}
