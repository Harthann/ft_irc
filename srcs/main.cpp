#include "ft_irc.hpp"
#include "Socket.hpp"
#include "utils.hpp"
#include "User.hpp"
#include "Commands.hpp"
#include "Addr.hpp"
#include "Registration.hpp"
#include "Channel_Registration.hpp"
#include "mode.hpp"
#include "commands_prototypes.hpp"
#include "Server.hpp"

Server *server_addr;

void	pong_response(Commands &, Socket *client, Server &server)
{
	std::cout << "PONG RESPONS SEND" << std::endl;
	client->bufferize("PONG " + server.IP() + "\r\n");
}

void	exit_server(Commands &cmd, Server &server, Socket *client)
{
	User *user = check_user(server.getClients(), client);
	
	if (user)
	{
		if (user->flagIsSet(OPERATOR_FLAG))
			server.Stop();
		else
			client->bufferize(":" + server.getServerName() + REPLY(ERR_NOPRIVILEGES) + ":Permission Denied- You're not an IRC operator");
	}
	else
		server.Stop();
}

void	quit_server(Socket *client, Server &server, Commands &cmd)
{
	User *temp = check_user(server.getClients(), client);

	temp->partChannels();
	if(cmd.length() > 1)
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
			client->bufferize(":" + server.getServerName() + REPLY(ERR_ALREADYREGISTRED) + "Password can't be changed");
	}
	else if (!cmd.name().compare("SERVER"))
		server.setProxy(cmd, client);
	ret = already_register(client, server);
	if(!ret)
	{
		add_user(client, temp_users, cmd, server);
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
	if (!cmd.isValid()) {
		std::cout << "Command format invalid" << std::endl;
		client->bufferize("Command format invalid");
	}
	if (cmd.name() == "PASS" || cmd.name() == "SERVER" || cmd.name() == "NICK" || cmd.name() == "USER")
		identification(cmd, client, server, temp_users);
	else if (cmd.name() == "DIE")
		exit_server(cmd, server, client);
	else if(cmd.name() == "QUIT")
		quit_server(client, server, cmd);
	else if (!server.isRegister(client))
		client->bufferize(":" + server.getServerName() + REPLY(ERR_NOTREGISTERED) + " *" + ":You have not registered");
	else if(cmd.name() == "JOIN")
		add_to_channel(cmd, client, server);
	else if (cmd.name() == "NAMES")
		names_command(cmd, client, server);
	else if (cmd.name() == "LIST")
		list_command(cmd, client, server);
	else if(cmd.name() == "TOPIC")
		topic_command(cmd, client, server);
	else if(cmd.name() == "VERSION")
		version_command(cmd, client, server);
	else if(cmd.name() == "PART")
		part_from_channel(cmd, client, server);
	else if(cmd.name() == "QUIT")
		quit_server(client, server, cmd);
	else if(cmd.name() == "MODE")
		mode_parser(cmd, client, server);
	else if (cmd.name() == "PRIVMSG")
		messages_command(cmd, client, server);
	else if (cmd.name() == "NOTICE")
		notice_command(cmd, client, server);
	else if (cmd.name() == "AWAY")
		away_command(cmd, client, server);
	else if (cmd.name() == "INVITE")
		InvitingUser(cmd, client, server);
	else if(cmd.name() == "KICK")
		KickUser(cmd, client, server);
	else if (cmd.name() == "PONG")
	{
		client->setPinged();
		server.logString("Pong received");
	}
}

void	server_loop(int port, std::string password, host_info &host)
{	
	Server					*server = NULL;
	Server::clients_vector	client_list;
	std::string				datas;
	std::vector<User> 		temp_users;

	try {
		server = new Server(port, password);
		server_addr = server;
		if (host.host.sin_zero[0] == 'h')
			server->setHost(host);
		server->logString("Server construction done");
		while (server->IsRunning()) {
				server->update();
			if (server->Select() < 0)
				break ;
			client_list = server->getSocketList();
			for (Server::client_it it = client_list.begin(); it != client_list.end(); ++ it)
			{
				if (server->readable(*it)) {
					if (server->IsMaster(*it))
						server->add((*it)->Accept());
					else {
						datas = (*it)->Receive();
						if (!datas.length()) {
							server->remove(*it);
							break ;
						}
						else
							command_dispatcher(datas, *it, *server, temp_users);
					}
				}
				if (server->writeable(*it))
					(*it)->flushWrite();
			}
			server->checkChannels();
			server->ping();
		}
		server->logString("Closing server");
		// std::cout << server->timer() << "\tClosing server" << std::endl;
		delete server;
	}
	catch (se::ServerException &e) {
		if (server)
			delete server;
		throw e;
	}
}

static void	set_signals()
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGABRT, signal_handler);
	signal(SIGFPE, signal_handler);
	signal(SIGQUIT, signal_handler);
}

int main(int ac, char **av)
{
	int					port;
	std::string 		pass;
	host_info			host;


	try {
		server_addr = NULL;
		set_signals();
		host = parse_info(ac, av, port, pass);
		server_loop(port, pass, host);
	}
	catch (se::ServerException &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
