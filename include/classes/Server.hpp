#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "User.hpp"
#include <vector>
#include <sys/time.h>

class Server
{
	public:
		Server(int const &);
		Server(Server const &);
		Server &operator=(Server const&);
		~Server();

		Socket *Select();					// Wait for any readable connection
		void	add(Socket*);				// add a client to the list
		void	remove(Socket*);			// Remove a client on the list
		void	update();					// Update fd_set of all client still connected
		bool	IsMaster(Socket*);
		std::vector<User>	& getClients();
	protected:
		std::vector<Socket*>	users;
		std::vector<User>		clients;
		Socket					*master;
		fd_set					readfds;
		int						max_fd;
};

#endif
