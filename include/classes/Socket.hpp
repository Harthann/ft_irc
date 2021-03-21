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

class Socket
{
	public:
		Socket(int port = 0);
		Socket(Socket const &);
		Socket &operator=(Socket const&);
		~Socket();

		const int &		getSocket() const;
		bool			Bind();
		bool			Listen();
		Socket			*Accept();
		void			Send(std::string message = "NOTICE {HOME_CHANNEL} Welcome to this server\n");
		std::string		Receive();

		struct InvalidAccept : public std::exception {
			char const *what() const throw() {
				return ("Error accepting new connection");
			}
		};

		struct InvalidBind : public std::exception {
			char const *what() const throw() {
				return ("Error binding new socket");
			}
		};

		struct SelectionError : public std::exception {
			char const *what() const throw() {
				return ("Error selecting incoming request");
			}
		};

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
