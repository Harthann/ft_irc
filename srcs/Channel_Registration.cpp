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
bool	CheckKey(Commands &cmd, Channel *temp, User *user)
{
	if(cmd.length() == 3)
	{
		if(cmd[2] == temp->getKey())
			return true;
	}
	std::string msg = ":" + temp->getServerName() + ERR_BADCHANNELKEY + user->getUser() + " " + temp->getName() + " :Cannot join channel (+k) \n";
	user->getSocketPtr()->bufferize(msg);
	return false;
}

void	add_member(User *user, Server &server, Commands &cmd)
{
	Channel *temp;
	for (unsigned long i = 0; i < server.getChannels().size(); i++)
	{
		if(server.getChannels()[i]->getName() == cmd[1])
		{
			temp = server.getChannels()[i];
			if ((!temp->IsInviteOnly() && !temp->getUserByName(user->getNickname()) && !temp->KeyIsSet()) || (temp->IsInviteOnly() && temp->CheckIfInvited(user)) || (temp->KeyIsSet() && CheckKey(cmd, temp, user)))
			{
				temp->addUser(user);
				user->ActiveChannel(temp);
			}
			else if((temp->IsInviteOnly() && !temp->CheckIfInvited(user)))
			{
				std::string msg = ":" + temp->getServerName() + ERR_INVITEONLYCHAN + user->getUser() + " " + temp->getName() + "\n";
				user->getSocketPtr()->bufferize(msg);
			}
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
		add_member(current_user, server, cmd);
	cmd.setFrom(current_user->getNickname());
}

void	part_from_channel(Commands cmd, Socket *client, Server &server)
{
	User *current_user;
	current_user = check_user(server.getClients(), client);
	if(channel_exist(cmd[1], server))
		current_user->partChannel(cmd[1]);
}
