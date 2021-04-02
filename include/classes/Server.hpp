#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "ft_irc.hpp"
#include <vector>
#include <map>
#include <sys/time.h>
#include "server_except.hpp"
#include <fstream>


//  irc.rizon.no
//	chat.freeenode.net
// irc.ircnet.com

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
		bool	readable(Socket *x) const;
		bool	writeable(Socket *x) const;
		void	redirect(std::string datas, Socket *client);
		std::fstream	&logStream();
		void			logString(std::string to_log);

		// char	&setClientType(Socket *);

		// typedef std::map<Socket*, char>::iterator		client_it;
		// typedef std::map<Socket*, char>::const_iterator	const_client_it;
		typedef std::vector<Socket*>::iterator		client_it;
		typedef std::vector<Socket*>::const_iterator	const_client_it;

	protected:
		std::fstream irc_log;
		
		Socket					*master;
		// std::map<Socket*, char>	clients;
		std::vector<Socket*>	clients;
		Socket					*host;
		
		fd_set					readfds;
		int						max_fd;
		
		fd_set					writefds;

		std::string				server_password;
		bool					state;
		bool					proxy;
};

#endif
