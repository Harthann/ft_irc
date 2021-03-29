#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "ft_irc.hpp"
#include <vector>
#include <sys/time.h>
#include "server_except.hpp"

//  irc.rizon.no

class Server
{
	public:
		Server(int const &, std::string = "");
		Server(Server const &);
		Server &operator=(Server const&);
		~Server();

		Socket *Select();					// Wait for any readable connection
		void	add(Socket*);				// add a client to the list
		void	remove(Socket*);			// Remove a client on the list
		void	update();					// Update fd_set of all client still connected
		void	setHost(host_info &);
		void	Stop();

		std::string	IP() const;
		bool	IsMaster(Socket*);
		bool	IsHost(Socket *) const;
		bool	IsRunning() const;
		bool	IsProxy() const;

		Socket *operator[](int i) {
			if (i == -1)
				return (host);
			if (!users.empty())
				return users[i];
			return nullptr;
		};


	protected:
		std::vector<Socket*>	users;
		Socket					*master;
		Socket					*host;
		std::string				server_password;
		
		fd_set					readfds;
		int						max_fd;
		
		bool					state;
		bool					proxy;
};

#endif
