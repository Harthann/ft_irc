#include <iostream>
#include "ft_irc.hpp"
#include "Socket.hpp"
#include <sstream>
#include <errno.h>
#include <string.h>
#include <vector>
#include <sys/time.h>
#include "Server.hpp"
#include "User.hpp"
#include "Registration.cpp"

void	server_loop(int port)
{
	try
	{
		// Socket	master(port);
		Server	server(port);
		Socket	*curr_client;
		std::string datas;
		std::vector<User> temp_users;

		while (1) {
			server.update();
			curr_client = server.Select();
			if (server.IsMaster(curr_client))
				server.add(curr_client->Accept());
			else {
				datas = curr_client->Receive();
				if (!datas.length())
					server.remove(curr_client);
				else if (!datas.compare("exit\n"))
				{
					std::cout << "Leaving server loop" << std::endl;
					break ;
				}
				else
				{
					add_user(curr_client, temp_users, datas);
					std::cout << "Received : " << datas << std::endl;
				}
			}
		}
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		throw e;
	}
}

int main(int ac, char **av)
{
	int port;
	std::string str(av[1]);

	std::istringstream(str) >> port;
	try {
		server_loop(port);
	}
	catch (std::exception &e) {
		// std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
