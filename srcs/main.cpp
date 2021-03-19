#include <iostream>
#include "ft_irc.hpp"
#include "Socket.hpp"
#include <sstream>
#include <errno.h>
#include <string.h>
#include <vector>
#include <sys/time.h>
#include "Server.hpp"

void	server_loop(int port)
{
	try
	{
		Socket	master(port);
		Server	server(master);
		Socket	curr_client;
		std::string datas;

		while (1) {
			server.update();
			curr_client = server.Select();
			if (curr_client == master)
				server.add(master.Accept());
			else {
				datas = curr_client.Receive();
				std::cout << datas << std::endl;
				if (!datas.length())
					server.remove(curr_client);
				else if (!datas.compare("exit\n"))
					break ;
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
