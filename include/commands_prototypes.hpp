#ifndef COMMANDS_PROTOTYPES_HPP
#define COMMANDS_PROTOTYPES_HPP

#include "Socket.hpp"
#include "Commands.hpp"
#include "Server.hpp"
#include "Channel_Registration.hpp"
#include "numeric_replies.hpp"
#include "utils.hpp"

void	topic_command(Commands &cmd, Socket *client, Server &server);
void	names_command(Commands &cmd, Socket *client, Server &server);
void	messages_command(Commands &cmd, Socket *client, Server &server);
void	away_command(Commands &cmd, Socket *client, Server &server);

#endif
