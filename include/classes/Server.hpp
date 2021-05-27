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
#include <string>
#include "numeric_replies.hpp"

//  irc.rizon.no
//	chat.freeenode.net
//	irc.ircnet.com

# define PING_FREQUENCY 20
# define SELECT_TIMEOUT 10

class Channel;

class Server
{
	public:
		Server(int const &, std::string = "");
		Server(Server const &);
		Server &operator=(Server const&);
		~Server();

		void					Stop();
		void					redirect(Commands &, Socket *);
		void					flushClient();
		void					ping();
		
		/****************************************************************/
		/*				logs function to keep a trace of events			*/
		/****************************************************************/	
		std::fstream			&logStream();
		void					logString(std::string);


		/****************************************************************/
		/*				Select linked function							*/
		/****************************************************************/
		int						Select();
		void					update();
		void					fdSet(std::vector<Socket*> &);
		void					fdSet(std::vector<Proxy> &);
		void					fdSet(std::vector<User*> &);
		bool					readable(Socket *) const;
		bool					writeable(Socket *) const;

		/****************************************************************/
		/*			Controls Socket flows inside server					*/
		/****************************************************************/
		/*						ADDERS									*/
		void					add(Socket*);
		void					setHost(host_info &);
		void					setProxy(Commands &, Socket *);
		void					addUser(User *);
		void					addChannel(Channel *Ch);
		void					addUnavailableNickname(std::string nick);

		/*						REMOVERS								*/
		void					remove(Socket*);
		void					removeSocket(Socket *);
		void					delete_user(User *user, std::string msg1);
		void					removeServer(Socket*);

		/****************************************************************/
		/*							Getters								*/
		/****************************************************************/
		std::vector<User*>		&getClients();
		User					*getUserByName(std::string);
		std::vector<Channel *>	&getChannels();
		std::string				&getServerName();
		std::vector<Socket *>	&getSocketList();
		std::vector<std::string>	&getUnavailableNicknames();

		/****************************************************************/
		/*						Information function					*/
		/****************************************************************/
		std::string				IP() const;
		bool					IsMaster(Socket*);
		bool					IsRunning() const;
		bool					isRegister(Socket *);
		bool					timedOut(Socket *);
		void					checkChannels();
		time_t					timer() const;
		bool					IsUserOnServer(std::string nickname);

		/****************************************************************/
		/*						Vector typedef							*/
		/****************************************************************/
		typedef	std::vector<Socket*>			clients_vector;
		typedef	std::vector<User *>				user_vector;
		typedef	std::vector<Channel *>			channel_vector;
		typedef	std::vector<Proxy>				proxy_vector;
	
		/****************************************************************/
		/*						Iterator typedef						*/
		/****************************************************************/
		typedef clients_vector::iterator		client_it;
		typedef clients_vector::const_iterator	const_client_it;
		typedef proxy_vector::iterator			proxy_it;
		typedef proxy_vector::const_iterator	const_proxy_it;
		typedef user_vector::iterator			user_it;
		typedef user_vector::const_iterator		const_user_it;
		typedef channel_vector::iterator		channel_it;
		typedef channel_vector::const_iterator	const_channel_it;
	
	private:
		/****************************************************************/
		/*						Block parser							*/
		/****************************************************************/
		std::string			__extract_block(std::fstream &);
		std::string			__process_block(std::string &, std::string);

	protected:
		std::fstream		irc_log;

/****************************************************************/
/*						Socket members							*/
/****************************************************************/
		Socket				*master;
		clients_vector		pending_clients;
		clients_vector		socket_list;
		proxy_vector		servers;
		user_vector			users;

		std::vector<std::string>	unavailable_nicknames;

		channel_vector		channels;
		
/****************************************************************/
/*						FDS BUFFER								*/
/****************************************************************/
		fd_set				readfds;
		fd_set				writefds;
		int					max_fd;
		
/****************************************************************/
/*						Server Info								*/
/****************************************************************/
		std::string			server_name;
		std::string			server_message;
		std::string			server_password;
		bool				state;
		int					timeout;
		time_t				last_ping;
		time_t				launched_time;

/****************************************************************/
/*						Private function						*/
/****************************************************************/
		std::string			__header();

};

#endif
