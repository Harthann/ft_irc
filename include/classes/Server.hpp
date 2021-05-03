#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "User.hpp"
#include "ft_irc.hpp"
#include "Proxy.hpp"
#include "Commands.hpp"
#include <vector>
#include <map>
#include <sys/time.h>
#include "server_except.hpp"
#include <fstream>

//  irc.rizon.no
//	chat.freeenode.net
//	irc.ircnet.com

# define TIME_LIMIT 10.0
# define SET_TIMEOUT false
class Channel;

class Server
{
	public:
		Server(int const &, std::string = "");
		Server(Server const &);
		// Server &operator=(Server const&);
		~Server();

		std::vector<Socket*>	&Select();					// Wait for any readable connection
		void					add(Socket*);				// add a client to the list
		void					remove(Socket*);			// Remove a client on the list
		void					update();					// Update fd_set of all client still connected
		std::vector<User>		&getClients();
		void					setHost(host_info &);
		void					Stop();

		std::string				IP() const;
		bool					IsMaster(Socket*);
		bool					IsRunning() const;

		bool					readable(Socket *) const;
		bool					writeable(Socket *) const;
		void					redirect(Commands &, Socket *);
		void					flushClient(); // Try to send pending message for each client
		
		// logs function to keep a trace of events
		
		std::fstream			&logStream();
		void					logString(std::string);

		//	fdSet functions update socket list for select
		void					fdSet(std::vector<Socket*> &);
		void					fdSet(std::vector<Proxy> &);
		void					fdSet(std::vector<User> &);

		//	Controls Socket flows inside server
		bool					isRegister(Socket *);
		void					removeSocket(Socket *);
		void					setProxy(Commands &, Socket *);
		void					addUser(User &);
		bool					timedOut(Socket *);
		std::vector<Channel>	&getChannels();
		void					addChannel(Channel &Ch);


		typedef	std::vector<Socket*>					clients_vector;
		typedef	std::vector<User>						user_vector;
		typedef	std::vector<Channel>					channel_vector;
		typedef	std::vector<Proxy>						proxy_vector;
	
		typedef clients_vector::iterator				client_it;
		typedef clients_vector::const_iterator			const_client_it;

		typedef proxy_vector::iterator					proxy_it;
		typedef proxy_vector::const_iterator			const_proxy_it;

		typedef user_vector::iterator					user_it;
		typedef user_vector::const_iterator				const_user_it;

		typedef channel_vector::iterator				channel_it;
		typedef channel_vector::const_iterator			const_channel_it;

	protected:
		std::fstream irc_log;
		
		Socket					*master;

		clients_vector			pending_clients;
		clients_vector			socket_list;
		proxy_vector			servers;
		user_vector				users;

		channel_vector			channels;
		
		fd_set					readfds;
		int						max_fd;
		
		fd_set					writefds;

		std::string				server_password;
		bool					state;
};

#endif
