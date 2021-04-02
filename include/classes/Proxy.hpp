#ifndef PROXY_HPP
#define PROXY_HPP

#include "Socket.hpp"
#include <ctime>

class Proxy
{
	public:
		Proxy(Socket * = NULL, int = 1, std::string = NULL);
		~Proxy();
	protected:
	private:
		Proxy(Proxy const &);
		Proxy &operator=(Proxy const&);

		Socket		*socket;
		time_t		timestamp;
		int			hopcount;
		std::string	name;

};

#endif
