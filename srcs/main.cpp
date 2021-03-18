#include <iostream>
#include "ft_irc.hpp"
#include "Socket.hpp"
#include <sstream>
#include <errno.h>
#include <string.h>

int main(int ac, char **av)
{
	int port;
	std::string str(av[1]);
	std::istringstream(str) >> port;

	Socket master(port);
	std::cout << master.getSocket() << std::endl;
	if (master.Listen())
		return (1);
	// {
	// 	activity -= select() 
	// 	if (nmaster)
	// 	{}
	// 	for (i = 0; i < client_max; ++i) {
	// 		if (FT_ISSET(*client[i], readfds))
	// 		{
	// 			message = client.read();
	// 			handle_message();
	// 		}
	// 	}
	// }
	Socket user = master.Accept();
	user.Send();
	std::cout << "New user connected\n";
	std::cout << "Hello_world" << std::endl;
	return (0);
}
