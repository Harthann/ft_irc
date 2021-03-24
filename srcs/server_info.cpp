#include "ft_irc.hpp"

#include <vector>

static	void	fill_host(std::string &param, struct sockaddr_in &addr)
{
	int ret;
	struct addrinfo	*list;
	struct addrinfo	hints;
	std::string ip;
	std::string port;
	std::string pass;

	ip = param.substr(0, param.find(':'));
	if (param.find(':') == param.rfind(':')) {
		port = param.substr(param.find(':') + 1, param.length());
	}
	else {
		port = param.substr(param.find(':') + 1, param.rfind(':') - param.find(':') - 1);
		pass = param.substr(param.rfind(':') + 1, param.length());
	}
	std::cout << "ip : " << ip << std::endl;
	std::cout << "port : " << port << std::endl;
	std::cout << "pass : " << pass << std::endl;
	hints.ai_socktype = SOCK_STREAM;
	ret = getaddrinfo(ip.c_str(), port.c_str(), &hints, &list);
	if (ret)
		std::cout << gai_strerror(ret) << std::endl;
	else
	{
		std::cout << "Success" << std::endl;
		while (list) {
			std::cout << inet_ntoa(reinterpret_cast<struct sockaddr_in*>(list->ai_addr)->sin_addr) << std::endl;
			list = list->ai_next;
		}
		freeaddrinfo(list);
	}
	throw WrongHostFormat();
}

struct sockaddr_in	parse_info(int ac, char **av, int &port, std::string &pass)
{
	struct sockaddr_in			addr;
	std::vector<std::string>	params;

	// std::istringstream(str) >> port;

	if (ac < 2 || ac > 3)
		throw WrongArgumentNumber();

	for (int i = 1; i < ac; ++i)
		params.push_back(av[i]);

	if (params[0].find('.') != std::string::npos || params[0].find(':') != std::string::npos) {
		try { fill_host(params[0], addr); } catch (std::exception &e) { throw e; }
	}
	return (addr);
		
}
