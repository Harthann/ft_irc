#include "ft_irc.hpp"
#include "Server.hpp"

void	signal_handler(int signal)
{
	// server_addr->logStream() << std::endl;
	// std::cout << std::endl;
	if (signal == SIGINT || signal == SIGQUIT)
		server_addr->Stop();
	// if (signal == SIGPIPE)
	// 	std::cout << "Sigpipe catch" << std::endl;
	
}


