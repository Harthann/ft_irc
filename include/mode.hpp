#ifndef MODE_HPP
# define MODE_HPP

# include "Socket.hpp"
# include "User.hpp"
# include "Server.hpp"
# include "Commands.hpp"
# include "Channel_Registration.hpp"
# include <vector>
# include <iostream>

void	mode_parser(Commands &cmd, Socket *client, Server &server);
void	RemoveMode(Commands &mode, Channel *channel ,User *user);
void	AddMode(Commands &mode, Channel *channel ,User *user);


#endif

