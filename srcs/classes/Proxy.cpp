#include "Proxy.hpp"

Proxy::Proxy(Socket *x, int hop, std::string name)
: socket(x), hopcount(hop), name(name)
{
	timestamp = time(NULL);
}

Proxy::Proxy(Proxy const &)
{

}

Proxy	&Proxy::operator=(Proxy const &)
{
	return (*this);
}

Proxy::~Proxy()
{

}
