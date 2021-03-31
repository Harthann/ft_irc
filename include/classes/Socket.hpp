#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "ft_irc.hpp"
#include "server_except.hpp"
#include <iterator>

class Socket
{
	public:
		Socket(int port = 0, std::string = "", std::string IP = "127.0.0.1");
		Socket(host_info &);
		Socket(Socket const &);
		Socket &operator=(Socket const&);
		~Socket();

		const int &		getSocket() const;
		sockaddr_in		getInfo() const;
		bool			Bind();
		bool			Connect();
		bool			Listen();
		std::string		IP() const;
		std::string		getHostName() const;
		Socket			*Accept();
		void			Send(std::string message = "NOTICE {HOME_CHANNEL} Welcome to this server\n");
		std::string		Receive();

		

		bool	operator==(Socket const& x) {
			return (x.socketfd == this->socketfd);
		};

		bool	operator!=(Socket const& x) {
			return (x.socketfd != this->socketfd);
		};

	protected:
		Socket(int fd, struct sockaddr_in addr, int addr_l);

		struct sockaddr_in	addr_info;
		int					addr_len;
		int					socketfd;
};

#endif
