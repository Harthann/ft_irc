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
#include <fstream>

#include <limits.h>
#include <float.h>

void	not_implemented(std::string &, Socket *, Server&)
{
	std::cout << "This command isn't implemented yet" << std::endl;
}

void	pong_response(std::string &, Socket *client, Server &server)
{
	client->Send("PONG " + server.IP() + "\r\n");
	std::cout << "PONG RESPONS SEND" << std::endl;
}

void	notice_command(std::string &datas, Socket *, Server &server)
{
	std::cout << datas;
}

void	exit_server(std::string &, Socket *, Server &server)
{
	server.Stop();
}

void	add_server_proxy(std::string &datas, Socket *client, Server &server)
{
	server.setClientType(client) = 's';
}

// void	redirect_datas(std::string &datas, Socket *client, Server &server)
// {
// 	if (server.IsHost(client) && server[0]) {
// 		server[0]->Send(datas);
// 	} else
// 		server[-1]->Send(datas);
// }

void	command_dispatcher(std::string &datas, Socket *client, Server &server, std::fstream& irc_log)
{
	// static Commands cmd;
	std::string			buff;
	std::vector<std::string>	cmd;

	cmd = utils::split_params(datas);
	for (std::vector<std::string>::iterator it = cmd.begin(); it != cmd.end(); ++it)
		std::cout << *it << std::endl;
	// if (server.IsProxy())
	// 	redirect_datas(datas, client, server);

	// cmd[buff](datas, client, server);
	// irc_log << datas;
	// std::cout << "{" << datas << "}" << std::endl;
}

void	server_loop(int port, std::string password, host_info &host)
{
	std::fstream irc_log;
	irc_log.open("irc_log", std::ios::out);
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
			if (server.IsMaster(curr_client))
				server.add(curr_client->Accept());
			else if (server.readable(curr_client)) {
				datas = curr_client->Receive();
				if (!datas.length())
					server.remove(curr_client);
				else
					command_dispatcher(datas, curr_client, server, irc_log);
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
