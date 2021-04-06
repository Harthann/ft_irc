#ifndef PROXY_HPP
#define PROXY_HPP

#include "Socket.hpp"
#include <ctime>

class Proxy
{
	public:
		Proxy(Socket * = NULL, int = 1, std::string = NULL);
		Proxy(Proxy const &);
		Proxy &operator=(Proxy const&);
		~Proxy();

		time_t	getTime() const;
		int		getSocket() const;
		Socket	*getSocketPtr();
		
	protected:
	private:

		Socket		*socket;
		int			hopcount;
		std::string	name;

};

#endif
