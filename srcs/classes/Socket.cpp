#include "Socket.hpp"

Socket::Socket(int port)
{
	this->socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr_info.sin_family = AF_INET;
	addr_info.sin_port = htons(port);
	addr_info.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->addr_len = sizeof(this->addr_info);
	this->Bind();
}

Socket::Socket(int fd, struct sockaddr_in addr, int addr_l)
{
	this->socketfd = fd;
	this->addr_info = addr;
	this->addr_len = addr_l;
}

Socket::Socket(Socket const &x)
{
	this->addr_info = x.addr_info;
}

Socket	&Socket::operator=(Socket const &x)
{
	this->addr_info = x.addr_info;
	return (*this);
}

Socket::~Socket()
{

}

const int	&Socket::getSocket() const {
	return (this->socketfd);
}

bool	Socket::Bind()
{
	int		ret;
	ret = bind(this->socketfd, reinterpret_cast<struct sockaddr*>(&this->addr_info), addr_len);
	if (ret)
		std::cout << "Something went wrong at binding phase for socket : " << this->socketfd;
	return (static_cast<bool>(ret));
}

bool	Socket::Listen() {
	int ret;

	ret = listen(this->socketfd, 3);
	if (ret < 0)
		std::cout << "Something went wrong at listening phase for socket : " << this->socketfd;
	return (static_cast<bool>(ret));
}

Socket	Socket::Accept()
{
	int	new_fd;

	new_fd = accept(this->socketfd, reinterpret_cast<struct sockaddr*>(&this->addr_info),
									reinterpret_cast<socklen_t*>(&this->addr_len));
	if (new_fd < 0)
		throw Socket::InvalidAccept();
	return (Socket(new_fd, this->addr_info, this->addr_len));
}

void	Socket::Send(std::string message)
{
	char str[256];
	if (send(this->socketfd, message.c_str(), message.length(), 0) != message.length())
		std::cout << "Error sending message to client : " << this->socketfd << std::endl;
	recv(this->socketfd, str, 256, 0);
	std::string tmp(str);
	std::cout << "Recieved back : " <<  str << std::endl;
}

// std::string	&Socket::Read() {
// 	std::string ret;

	
// }