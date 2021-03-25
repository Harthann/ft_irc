#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "ft_irc.hpp"
#include <vector>
#include <sys/time.h>

class Server
{
	public:
		Server(int const &, std::string = "");
		Server(int const &, std::string &, host_info&);
		Server(Server const &);
		Server &operator=(Server const&);
		~Server();

		Socket *Select();					// Wait for any readable connection
		void	add(Socket*);				// add a client to the list
		void	remove(Socket*);			// Remove a client on the list
		void	update();					// Update fd_set of all client still connected
		std::string	IP() const;
		bool	IsMaster(Socket*);
		bool	IsHost(Socket *) const;
		void	Stop();
		bool	IsRunning() const;
		bool	IsProxy() const;

		Socket *operator[](int i) {
			if (i == -1)
				return (host);
			return users[i];
		};

	protected:
		std::vector<Socket*>	users;
		Socket					*master;
		Socket					*host;
		fd_set					readfds;
		int						max_fd;
		bool					state;
		bool					proxy;
};

#endif
