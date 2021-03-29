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

void	not_implemented(std::string &, Socket *, Server&)
{
	std::cout << "This command isn't implemented yet" << std::endl;
	// client->Send("NOTICE es Commands not allowed\r\n");
}

void	pong_response(std::string &, Socket *client, Server &server)
{
	client->Send("PONG " + server.IP() + "\r\n");
	std::cout << "PONG RESPONS SEND" << std::endl;
}

void	exit_server(std::string &, Socket *, Server &server)
{
	server.Stop();
}

void	command_dispatcher(std::string &datas, Socket *client, Server &server)
{
	static Commands cmd;
	std::stringstream ss(datas);
	std::string			buff;

	if (!server.IsHost(client))
		server[-1]->Send(datas);
	else if (server[0])
		server[0]->Send(datas);

	ss >> buff;
	cmd[buff](datas, client, server);
	std::cout << datas <<  std::endl;
}

#!/usr/bin/expect -f
spawn ssh -p2522 user42@localhost -t zsh
expect "assword:"
send "user42\r"
interact


void	server_loop(int port, std::string password, host_info &host)
{
	try
	{
		Server		server(port, password);
		Socket		*curr_client;
		std::string	datas;

		if (host.host.sin_zero[0] == 'h')
			server.setHost(host);
		while (server.IsRunning()) {
			server.update();
			curr_client = server.Select();
			std::cout << "Right after select " << curr_client <<  std::endl;
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
	catch (se::ServerException &e) {
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
	catch (se::ServerException &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
