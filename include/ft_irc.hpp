#ifndef FT_IRC_HPP
#define FT_IRC_HPP

//	Socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
//	Net header
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctime>

#include <sys/time.h>
// IO header
#include <iostream>
#include <unistd.h>

// Other
#include <string.h>
#include <exception>

// STL header
#include <vector>

//	Illegal header
#include <errno.h>

#ifndef nullptr
# define nullptr 0
#endif


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

struct host_info
{
	struct sockaddr_in	host;
	std::string			pass;
};


host_info					parse_info(int ac, char **av, int &port, std::string &pass);
namespace utils {
	std::vector<std::string>	split_params(std::string &datas);
	std::string					reunite_params(std::vector<std::string> &params);
}


#endif
