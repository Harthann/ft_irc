#include "Registration.hpp"

void	validation(std::vector<User> &temp_user, Socket *client)
{
	for (unsigned int i = 0; i < temp_user.size(); i++)
	{
		if(temp_user[i].getSocketPtr() == client)
		{
	//		client->Confirm("001  RPL_WELCOME Welcome to the Internet Relay Network " + temp_user[i].getNickname() + "!" + temp_user[i].getUser() + "@127.0.0.1");
//			client->Send("001 RPL_WELCOME   Welcome to the Internet Relay Network " + temp_user[i].getNickname() + "!" + temp_user[i].getUser() + "@localhost\n");

			break;
		}
	}
}

void	add_user(Socket *client, std::vector<User> &temp_user, Commands &cmd)
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
		temp_user[i].setDatas(cmd);
	else if(n == 0)
		temp_user.push_back(User(client, cmd));
//	client->Send("001 RPL_WELCOME [USER_NAME] Welcome to the server\n");
//	client->Confirm("001 WELCOME_MSG [USER_NAME] Welcome to the server");
	validation(temp_user, client);
}

int		already_register(Socket *client, Server &server)
{
	for(unsigned long i = 0; i < server.getClients().size(); ++i)
	{
		if(server.getClients()[i].getSocketPtr() == client)
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
			server.addUser(*it);
			it = temp_user.erase(it);
		}
	}
}
