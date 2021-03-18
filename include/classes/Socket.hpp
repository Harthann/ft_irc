#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

class Socket
{
	public:
		Socket(int port);
		Socket(Socket const &);
		Socket &operator=(Socket const&);
		~Socket();

		const int &		getSocket() const;
		bool			Bind();
		bool			Listen();
		Socket			Accept();
		void			Send(std::string message = "Welcome to this server\nsdfgsdgagdsfgsdfgf");
		std::string		Read();

		struct InvalidAccept : public std::exception {
			char const *what() const throw() {
				return ("Error accepting new connection");
			}
		};
	protected:
		Socket(int fd, struct sockaddr_in addr, int addr_l);

		struct sockaddr_in	addr_info;
		int					addr_len;
		int					socketfd;
};

#endif
