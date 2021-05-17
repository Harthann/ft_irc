#ifndef MODE_HPP
# define MODE_HPP

# include "Socket.hpp"
# include "User.hpp"
# include "Server.hpp"
# include "Commands.hpp"
# include <vector>
# include <iostream>

void	mode_parser(Command &cmd, Socket *client, Server &server);

#endif

