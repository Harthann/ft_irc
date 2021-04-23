#ifndef ADDR_HPP
#define ADDR_HPP

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

struct sockaddr_in;

class Addr : public sockaddr_in
{
	public:
		Addr();
		Addr(const char *, int, int);
		Addr(Addr const &);
		Addr &operator=(Addr const&);

		void	setIP(const char *);
		void	setIP(std::string &);
		void	setPort(int &);
		void	setHost(char );

		const char*	getIP() const;
		int			getPort() const;

		void	setType(int);
		int		addrlen() const;

		struct sockaddr*	as_sockaddr();
		struct sockaddr_in*	as_sockaddr_in();
		~Addr();
};

#endif
