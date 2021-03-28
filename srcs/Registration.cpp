#include "Socket.hpp"
#include "User.hpp"
#include "Server.hpp"
#include <vector>
#include <iostream>

void	add_user(Socket *client, std::vector<User> &temp_user, std::string datas)
{
	unsigned long i;
	int n = 0;
	for (i = 0; i < temp_user.size(); i++)
	{
		if(temp_user[i].getSocket() == client)
		{
			n = 1;
			break;
		}
	}
	if(n == 1 && !temp_user[i].getStatus())
		temp_user[i].setDatas(datas);
	else if(n == 0)
		temp_user.push_back(User(client, datas));
}

int		already_register(Socket *client, Server &server)
{
	for(unsigned long i = 0; i < server.getClients().size(); ++i)
	{
		if(server.getClients()[i].getSocket() == client)
			return (1);
	}
	return (0);
}

void	update_server_user(std::vector<User> &temp_user, Server &server)
{
	std::vector<User>::iterator ite = temp_user.end();
	for (std::vector<User>::iterator it = temp_user.begin(); it != ite; ++it)
	{
		if (it->getStatus() == 1)
		{
			server.getClients().push_back(*it);
			it = temp_user.erase(it);
		}
	}
}
