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
<<<<<<< HEAD
void	list_command(Commands &cmd, Socket *client, Server &server);
void	version_command(Commands &cmd, Socket *client, Server &server);
=======
void	away_command(Commands &cmd, Socket *client, Server &server);
void	notice_command(Commands &cmd, Socket *client, Server &server);
void	InvitingUser(Commands &cmd, Socket *client, Server &server);

>>>>>>> 44b796b961da4fab43e193a38f1793ecc1377b56

#endif
