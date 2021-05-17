#include <iostream>
#include "ft_irc.hpp"
#include "Socket.hpp"
#include <sstream>
#include "utils.hpp"
#include <errno.h>
#include <string.h>
#include <vector>
#include <sys/time.h>
#include "User.hpp"
#include "Commands.hpp"
#include "Addr.hpp"
#include "Registration.hpp"
#include "Channel_Registration.hpp"
#include "commands_prototypes.hpp"

#include <limits.h>
#include <float.h>

void	pong_response(Commands &, Socket *client, Server &server)
{
	client->bufferize("PONG " + server.IP() + "\r\n");
	std::cout << "PONG RESPONS SEND" << std::endl;
}

void	exit_server(Commands &, Socket *, Server &server)
{
	server.Stop();
}

void	quit_server(Socket *client, Server &server, Commands &cmd)
{
	User *temp = check_user(server.getClients(), client);

	temp->partChannels();
	server.delete_user(temp, cmd[1]);
	delete temp;
}

void	identification(Commands &cmd, Socket *client, Server &server, std::vector<User> &temp_users)
{
	int ret = 0;

	if (!cmd.name().compare("PASS"))
	{
		if (!server.isRegister(client))
			client->setPassword(cmd[1]);
		else
			client->bufferize("462 ERR_ALREADYREGISTRED Password can't be changed");
	}
	else if (!cmd.name().compare("SERVER"))
		server.setProxy(cmd, client);
	ret = already_register(client, server);
	if(!ret)
	{
		add_user(client, temp_users, cmd);
		update_server_user(temp_users, server);
	}
}

/****************************************************/
/**					HELLLOOOOOOOOOOO		 		*/
/****************************************************/

void	command_dispatcher(std::string &datas, Socket *client, Server &server, std::vector<User> &temp_users)
{
	Commands cmd(datas);
	std::string	cmd_name;

	server.logString(cmd.as_string());
	std::cout << cmd.as_string() << std::endl;
	if (!cmd.isValid()) {
		std::cout << "Command format invalid" << std::endl;
		client->bufferize("Command format invalid");
	}
	if (cmd.name() == "PASS" || cmd.name() == "SERVER" || cmd.name() == "NICK" || cmd.name() == "USER")
		identification(cmd, client, server, temp_users);
	else if (!cmd.name().compare("DIE") || !cmd.name().compare("DIE\n"))
		exit_server(cmd, client, server);
	else if(cmd.name() == "QUIT")
		quit_server(client, server, cmd);
	else if (!server.isRegister(client))
		client->bufferize(":" + server.getServerName() + " " + utils::itos(ERR_NOTREGISTERED) + " *" + ":You have not registered");
	else if(cmd.name() == "JOIN")
		add_to_channel(cmd, client, server);
	else if(cmd.name() == "TOPIC")
		topic_command(cmd, client, server);
	else if(cmd.name() == "PART")
		part_from_channel(cmd, client, server);
}

void	server_loop(int port, std::string password, host_info &host)
{	
	try {
		Server					server(port, password);
		Server::clients_vector	client_list;
		std::string				datas;
		std::vector<User> 		temp_users;

		if (host.host.sin_zero[0] == 'h')
			server.setHost(host);
		std::cout << "Server construction done" << std::endl;
		while (server.IsRunning()) {
				server.update();
			client_list = server.Select();
			for (Server::client_it it = client_list.begin(); it != client_list.end(); ++ it)
			{
				if (server.readable(*it)) {
					std::cout << "Read signal received on socket : " << (*it) << std::endl;
					if (server.IsMaster(*it))
						server.add((*it)->Accept());
					else {
						datas = (*it)->Receive();
						if (!datas.length()) {
							server.remove(*it);
							break ;
						}
						else
							command_dispatcher(datas, *it, server, temp_users);
					}
				}
				if (server.writeable(*it))
					(*it)->flushWrite();
			}
			server.checkChannels();
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
