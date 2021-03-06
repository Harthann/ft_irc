#ifndef COMMANDS_PROTOTYPES_HPP
#define COMMANDS_PROTOTYPES_HPP

#include "Socket.hpp"
#include "Commands.hpp"
#include "Server.hpp"
#include "Channel_Registration.hpp"
#include "numeric_replies.hpp"
#include "utils.hpp"
#include "mode_bool.hpp"

void	topic_command(Commands &cmd, Socket *client, Server &server);
void	names_command(Commands &cmd, Socket *client, Server &server);
void	messages_command(Commands &cmd, Socket *client, Server &server);
void	list_command(Commands &cmd, Socket *client, Server &server);
void	version_command(Socket *client, Server &server);
void	away_command(Commands &cmd, Socket *client, Server &server);
void	notice_command(Commands &cmd, Socket *client, Server &server);
void	InvitingUser(Commands &cmd, Socket *client, Server &server);
void	KickUser(Commands &cmd, Socket *client, Server &server);
void	quit_server(Socket *&client, Server &server, Commands &cmd);
bool	kill_by_server(Commands &cmd, Server &server, std::string msg = "Duplicates nicknames");
void	kick_command(Commands &cmd, Socket *client, Server &server);
void	who_querry(Commands &cmd, Socket *client, Server &server);
void	oper_commands(Commands &cmd, Socket *client, Server &server);


#endif
