#include "ft_irc.hpp"
#include "Server.hpp"

void	signal_handler(int signal)
{
	server_addr->logStream() << std::endl;
	std::cout << std::endl;
	if (signal == SIGINT || signal == SIGQUIT)
		server_addr->Stop();
	if (signal == SIGSEGV) {
		server_addr->logString("Closing server");
		// exit_server(*server_addr);
		// raise(2);
		_exit(0);
	}
}


