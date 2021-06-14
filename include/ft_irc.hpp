#ifndef FT_IRC_HPP
#define FT_IRC_HPP

/*	Socket headers	*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

/*	Net header		*/
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctime>

#include <sys/time.h>
/* 	IO header		*/
#include <iostream>
#include <unistd.h>

/*		 Other		*/
#include <string.h>
#include <exception>
#include <signal.h>

/*	 STL header		*/
#include <vector>

/*	Illegal header	*/
#include <errno.h>

/************************************/
/*			SERVER EXCEPTION		*/
/************************************/
#include "server_except.hpp"

#ifndef nullptr
# define nullptr 0
#endif

struct WrongArgumentNumber : public se::ServerException
{
	WrongArgumentNumber() : ServerException("Wrong argument number") {};
};

struct WrongHostFormat : public se::ServerException
{
	WrongHostFormat() : ServerException("Wrong host format") {};
};

struct WrongPortFormat : public se::ServerException
{
	WrongPortFormat() : ServerException("Wrong Port format") {};
};

struct ErrorOpeningFile : public se::ServerException
{
	ErrorOpeningFile() : ServerException("Failed to open file") {};
};

struct WrongConfigFileFormat : public se::ServerException
{
	WrongConfigFileFormat() : ServerException("Wrong config file format") {};
};

struct UnimplementedFeature : public se::ServerException
{
	UnimplementedFeature() : ServerException("This feature is not implemented") {};
};

/************************************/
/*			DECLARATIONS			*/
/************************************/

class Server;
extern Server *server_addr;

struct host_info
{
	Addr	host;
	std::string			pass;
};

host_info	parse_info(int ac, char **av, int &port, std::string &pass);
void		signal_handler(int signal);

#endif
