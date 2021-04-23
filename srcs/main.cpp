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
#include "Addr.hpp"

#include <limits.h>
#include <float.h>

void	pong_response(Commands &, Socket *client, Server &server)
{
	client->Send("PONG " + server.IP() + "\r\n");
	std::cout << "PONG RESPONS SEND" << std::endl;
}

void	exit_server(Commands &, Socket *, Server &server)
{
	server.Stop();
}

void	identification(Commands &cmd, Socket *client, Server &server)
{
	if (!cmd.name().compare("PASS")) {
		client->setBuff(cmd[1]);
		std::cout << "Password set to : " << cmd[1] << std::endl;
	}
	else if (!cmd.name().compare("SERVER"))
		server.setProxy(cmd, client);
}

void	command_dispatcher(std::string &datas, Socket *client, Server &server)
{
	Commands cmd(datas);
	std::string	cmd_name;

	server.logString(cmd.as_string());
	std::cout << cmd.as_string() << std::endl;
	if (!cmd.isValid()) {
		std::cout << "Command format invalid" << std::endl;
		client->Send("Command format invalid");
	}
	if (cmd.name() == "PASS" || cmd.name() == "SERVER" || cmd.name() == "NICK")
		identification(cmd, client, server);
	else if (!cmd.name().compare("DIE") || !cmd.name().compare("DIE\n"))
		exit_server(cmd, client, server);
	// if (server.IsProxy())
	server.redirect(cmd, client);
}

void	server_loop(int port, std::string password, host_info &host)
{	
	try {
		Server		server(port, password);
		Socket		*curr_client;
		std::string	datas;

		if (host.host.sin_zero[0] == 'h')
			server.setHost(host);
		std::cout << "Server construction done" << std::endl;
		while (server.IsRunning()) {
			server.update();
			curr_client = server.Select();
			if (server.IsMaster(curr_client) && server.readable(curr_client))
				server.add(curr_client->Accept());
			else if (server.readable(curr_client)) {
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
		// std::cout << "Host ip : " << host.host.getIP() << std::endl;
		// std::cout << "Host port : " << host.host.getPort() << std::endl;
		// std::cout << "Host pass : " << host.pass << std::endl;
		// std::cout << "Host status "	<< host.host.sin_zero << std::endl;
		// std::cout << "Server port : " << port << std::endl;
		// std::cout << "Server pass : " << pass << std::endl;
		server_loop(port, pass, host);
	}
	catch (se::ServerException &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
