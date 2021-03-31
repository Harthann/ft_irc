#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>
#include "Socket.hpp"
#include "Server.hpp"
#include "tokens.hpp"


void	not_implemented(std::string &, Socket *, Server&);
void	pong_response(std::string &, Socket *, Server&);
void	exit_server(std::string &, Socket *, Server&);
void	add_server_proxy(std::string &, Socket *, Server&);


class Commands
{
	typedef void (*fcntl)(std::string &, Socket*, Server&);
	
	public:
		Commands();
		~Commands();

		fcntl	operator[](std::string &);
	private:
		Commands(Commands const &);
		Commands &operator=(Commands const&);

		std::map<std::string, fcntl>	list;
};

#endif
