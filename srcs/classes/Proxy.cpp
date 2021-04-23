#include "Proxy.hpp"

Proxy::Proxy(Socket *x, int hop, std::string name)
: socket(x), hopcount(hop), name(name)
{

}

Proxy::Proxy(Proxy const &x)
{
	this->socket = x.socket;
	this->hopcount = x.hopcount;
	this->name = x.name;
}

Proxy	&Proxy::operator=(Proxy const &x)
{
	this->socket = x.socket;
	this->hopcount = x.hopcount;
	this->name = x.name;
	return (*this);
}

Proxy::~Proxy()
{

}

time_t	Proxy::getTime() const
{
	return (socket->getTime());
}

int		Proxy::getSocket() const
{
	return (socket->getSocket());
}

Socket		*Proxy::getSocketPtr()
{
	return (socket);
}