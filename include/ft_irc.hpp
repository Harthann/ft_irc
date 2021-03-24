#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <exception>

struct WrongArgumentNumber : public std::exception
{
	const char	*what() const throw() {
		return ("Wrong argument number");
	};
};

struct WrongHostFormat : public std::exception
{
	const char	*what() const throw() {
		return ("Wrong host format");
	};
};

struct sockaddr_in	parse_info(int ac, char **av, int &port, std::string &pass);


#endif
