#include "Addr.hpp"

Addr::Addr()
: sockaddr_in()
{
	this->sin_family = AF_INET;
}

Addr::Addr(const char *ip, int port, int type)
: sockaddr_in()
{
	this->setIP(ip);
	this->setPort(port);
	this->sin_family = type;
}

Addr::Addr(Addr const &)
{
}

Addr	&Addr::operator=(Addr const &x)
{
	this->sin_addr = x.sin_addr;
	this->sin_family = x.sin_family;
	this->sin_port = x.sin_port;
	for (int i = 0; i < 8; ++i)
		this->sin_zero[i] = x.sin_zero[i];
	return (*this);
}

Addr::~Addr()
{

}

void	Addr::setIP(const char*x)
{
	this->sin_addr.s_addr = inet_addr(x);
}

void	Addr::setIP(std::string &x)
{
	this->sin_addr.s_addr = inet_addr(x.c_str());
}

void	Addr::setPort(int &x)
{
	this->sin_port = htons(x);
}

void	Addr::setHost(char c)
{
	this->sin_zero[0] = c;
}

const char*		Addr::getIP() const
{
	return (inet_ntoa((this->sin_addr)));
}

int				Addr::getPort() const
{
	return (ntohs(this->sin_port));
}

int		Addr::addrlen() const
{
	return (sizeof(sockaddr_in));
}

sockaddr	*Addr::as_sockaddr()
{
	return (reinterpret_cast<struct sockaddr*>(this));
}
