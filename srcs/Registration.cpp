#include "Registration.hpp"

void	add_user(Socket *client, std::vector<User> &temp_user, Commands &cmd, Server &server)
{
	unsigned long i;
	int n = 0;
	for (i = 0; i < temp_user.size(); i++)
	{
		if(temp_user[i].getSocketPtr() == client)
		{
			n = 1;
			break;
		}
	}
	if(n == 1 && !temp_user[i].getStatus())
		temp_user[i].setDatas(cmd, server.getServerName());
	else if(n == 0)
		temp_user.push_back(User(client, cmd, server.getServerName()));
}

int		already_register(Socket *client, Server &server)
{
	for(unsigned long i = 0; i < server.getClients().size(); ++i)
	{
		if(server.getClients()[i]->getSocketPtr() == client)
			return (1);
	}
	return (0);
}

void	update_server_user(std::vector<User> &temp_user, Server &server)
{
	std::vector<User>::iterator ite = temp_user.end();
	User *temp;
	for (std::vector<User>::iterator it = temp_user.begin(); it != ite; ++it)
	{
		if (it->getStatus() == 1)
		{
			temp = new User(*it);
			server.addUser(temp);
			it = temp_user.erase(it);
		}
	}
}
