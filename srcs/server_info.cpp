#include "ft_irc.hpp"

#include <sstream>


static	void	fill_host(std::string &param, Addr &addr, std::string &host_pass)
{
	int ret;
	struct addrinfo	*list;
	struct addrinfo	hints;
	std::string ip;
	std::string port;

	ip = param.substr(0, param.find(':'));
	if (param.find(':') == param.rfind(':')) {
		port = param.substr(param.find(':') + 1, param.length());
	}
	else {
		port = param.substr(param.find(':') + 1, param.rfind(':') - param.find(':') - 1);
		host_pass = param.substr(param.rfind(':') + 1, param.length());
	}
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = 0;
	ret = getaddrinfo(ip.c_str(), port.c_str(), &hints, &list);
	if (ret) {
		std::cout << gai_strerror(ret) << std::endl;
		throw WrongHostFormat();
	}
	else {
		addr = *reinterpret_cast<Addr*>(list->ai_addr); // to upgrade
		addr.setHost('h');
		freeaddrinfo(list);
	}
}

host_info	parse_info(int ac, char **av, int &port, std::string &pass)
{
	host_info					addr;
	std::vector<std::string>	params;

	if (ac < 2 || ac > 4)
		throw WrongArgumentNumber();
	for (int i = 1; i < ac; ++i)
		params.push_back(av[i]);
	if (params[0].find('.') != std::string::npos || params[0].find(':') != std::string::npos) {
		try {
			fill_host(params[0], addr.host, addr.pass);
			params.erase(params.begin());
		} catch (std::exception &e) { throw e; }
	}
	try {
		port = utils::stoi(params[0]);
	} catch (se::ServerException &e) { throw e; }
	if (params.size() == 2) 
		pass = params[1];
	return (addr);		
}
