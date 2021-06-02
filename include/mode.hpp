#ifndef MODE_HPP
# define MODE_HPP

# include "Socket.hpp"
# include "User.hpp"
# include "Server.hpp"
# include "Commands.hpp"
# include "Channel_Registration.hpp"
# include <vector>
# include <iostream>
# include "utils.hpp"
# include "mode_bool.hpp"

void	mode_parser(Commands &cmd, Socket *client, Server &server);
void	mode_add_or_remove(Commands &mode, Channel *channel ,User *user);
void	mode_channel(Commands &cmd, Socket *client, Server &server);
void	mode_user(Commands &cmd, Socket *client, Server &server);


#endif

