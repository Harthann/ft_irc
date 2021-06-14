#include "ft_irc.hpp"


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
			throw UnimplementedFeature();
		} catch (std::exception &e) { throw e; }
	}
	try {
		port = utils::stoi(params[0]);
	} catch (se::ServerException &e) { throw e; }
	if (params.size() == 2)
		pass = params[1];
	return (addr);
}
