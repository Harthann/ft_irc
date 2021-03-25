#include <iostream>
#include "ft_irc.hpp"
#include "Socket.hpp"
#include <sstream>
#include <errno.h>
#include <string.h>
#include <vector>
#include <sys/time.h>
#include "Server.hpp"
#include "Commands.hpp"

#include <limits.h>
#include <float.h>

void	not_implemented(std::string &, Socket *socket, Server&)
{
	std::cout << "This command isn't implemented yet" << std::endl;
	// client->Send("NOTICE es Commands not allowed\r\n");
}

void	pong_response(std::string &, Socket *client, Server &server)
{
	client->Send("PONG " + server.IP() + "\r\n");
	std::cout << "PONG RESPONS SEND" << std::endl;
}

void	exit_server(std::string &, Socket *client, Server &server)
{
	server.Stop();
}

void	command_dispatcher(std::string &datas, Socket *client, Server &server)
{
	if (server.IsProxy()) {
		if (server.IsHost(client))
			server[0]->Send(datas.c_str());
		else
			server[-1]->Send(datas.c_str());

	}
	static Commands cmd;
	// int				offset;
	std::stringstream ss(datas);
	std::string			buff;

	ss >> buff;
	cmd[buff](datas, client, server);
	std::cout << datas << std::endl;
}

void	server_loop(int port, std::string password, host_info	host)
{
	try
	{
		Server		server(port, password, host);
		Socket		*curr_client;
		std::string	datas;

		while (server.IsRunning()) {
			server.update();
			curr_client = server.Select();
			if (server.IsMaster(curr_client))
				server.add(curr_client->Accept());
			else {
				datas = curr_client->Receive();
				if (!datas.length())
					server.remove(curr_client);
				else
					command_dispatcher(datas, curr_client, server);
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
	int					port;
	std::string 		pass;
	host_info			host;

	try {
		host = parse_info(ac, av, port, pass);
		server_loop(port, pass, host);
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
