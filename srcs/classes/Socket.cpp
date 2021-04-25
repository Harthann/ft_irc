#include "Socket.hpp"
#include <fcntl.h>

Socket::Socket(int port, std::string , std::string IP)
: addr(), buff()
{
	time(&this->timestamp);
	const int opt = 1;
	this->socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!this->socketfd)
		throw se::SocketFailed();
	this->addr.setPort(port);
	this->addr.setIP(IP.c_str());
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setsockopt(this->socketfd, SOL_SOCKET, SO_TIMESTAMP, &opt, sizeof(opt));
	fcntl(this->socketfd, F_SETFL, O_NONBLOCK);
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	if (this->Bind())
		throw se::InvalidBind();
}

Socket::Socket(host_info &host)
: buff()
{
	const int opt = 1;
	time(&this->timestamp);
	this->socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!this->socketfd)
		throw se::SocketFailed();
	this->addr = host.host;
	std::cout << "Addrsss : " << this->addr.getIP() << std::endl;
	std::cout << "Port : " << this->addr.getPort() << std::endl;
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	setsockopt(this->socketfd, SOL_SOCKET, SO_TIMESTAMP, &opt, sizeof(opt));
}

Socket::Socket(int fd, Addr ad, int length)
{
	time(&this->timestamp);
	this->socketfd = fd;
	this->addr = ad;
}

Socket::Socket(Socket const &x)
{
	this->addr = x.addr;
	this->socketfd = x.socketfd;
	this->timestamp = x.timestamp;
}

Socket	&Socket::operator=(Socket const &x)
{
	this->addr = x.addr;
	this->socketfd = x.socketfd;
	return (*this);
}

Socket::~Socket()
{
	close(socketfd);
}

const int	&Socket::getSocket() const {
	return (this->socketfd);
}

bool	Socket::Bind()
{
	int		ret;
	int		error;

	errno = 0;
	ret = bind(this->socketfd, this->addr.as_sockaddr(), this->addr.addrlen());
	error = errno;
	if (ret) {
		std::cout << "Something went wrong at binding phase for socket : " << this->socketfd << std::endl;
		std::cout << strerror(error) << std::endl;
	}
	return (static_cast<bool>(ret));
}

bool	Socket::Connect()
{
	int		ret;
	int		error;

	errno = 0;
	ret = connect(this->socketfd, this->addr.as_sockaddr(), this->addr.addrlen());
	error = errno;
	if (ret) {
		std::cout << "Something went wrong at connection phase for socket : " << this->socketfd << std::endl;
		std::cout << strerror(error) << std::endl;
	}
	return (static_cast<bool>(ret));
}

bool	Socket::Listen() {
	int ret;

	ret = listen(this->socketfd, 3);
	if (ret < 0)
		std::cout << "Something went wrong at listening phase for socket : " << this->socketfd;
	return (static_cast<bool>(ret));
}

Socket	*Socket::Accept()
{
	int					new_fd;
	Addr				addr;
	int					tmp_addr_len = addr.addrlen();

	new_fd = accept(this->socketfd, addr.as_sockaddr(),
									reinterpret_cast<socklen_t*>(&tmp_addr_len));
	if (new_fd < 0)
		throw se::InvalidAccept();
	std::cout << "New clients detected : " << addr.getIP();
	std::cout << " port : " << addr.getPort() << std::endl;
	return (new Socket(new_fd, addr, tmp_addr_len));
}

//	Receive and Send actually use read/write but will probably switch
//	to Receive and Send respectively for an extra argument that set
//	a timeout option in case of error

void	Socket::Send(std::string message)
{
	// char str[256];
	write(this->socketfd, message.c_str(), message.length());
	// if (send(this->socketfd, message.c_str(), message.length(), 0) != message.length())
	// 	std::cout << "Error sending message to client : " << this->socketfd << std::endl;
	// recv(this->socketfd, str, 256, 0);
	// std::string tmp(str);
	// std::cout << "Recieved back : " <<  str << std::endl;
}

void	Socket::Confirm(std::string message)
{
	send(this->socketfd, message.c_str(), message.length(), MSG_CONFIRM);

}

std::string Socket::Receive()
{
	//	Actually reading char one by one to avoid looking passed the command line
	//	Can be change to a bigger read for performance but 
	//	will need to store in buffer all extra char readed
	//	as they can be part of the next command line
	std::string	ret;
	char		buffer;
	int			readed;

	do {
		readed = read(this->socketfd, &buffer, 1);
		// if (readed > 0)
		// 	buffer[readed] = '\0';
		// else
		// 	return ("");
		if (readed > 0)
			ret += buffer;
	} while (readed && *(ret.end() - 1) != '\n' && *(ret.end() - 2) != '\r');
	return (ret);
}

void			Socket::setBuff(std::string &x)
{
	this->buff = x;
}

std::string		&Socket::getBuff()
{
	return (this->buff);
}

std::string		Socket::IP() const
{
	return (this->addr.getIP());
}

Addr	Socket::getInfo() const {
	return this->addr;
}

std::string	Socket::getHostName() const
{
	//	Probably not necessary and actually not working well
	//	This function will eventually be deleted if no solution found
	//	The goal is found host name base on ip address
	//	Ex : ip = 127.0.0.1 will give "localhost" as return value
	return ("");
}

time_t		Socket::getTime() const
{
	return (this->timestamp);
}

std::string	Socket::strTime() const
{
	std::string ret;

	ret =   asctime(gmtime(&this->timestamp));
	return ret;
}

std::string Socket::flush()
{
	std::string ret;
	char		buffer;
	int			readed;

	do {
		readed = read(this->socketfd, &buffer, 1);
		if (readed > 0)
			ret += buffer;
	} while (readed > 0);
	return ret;
}

/*
/join channelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelooochannelllllllkllkkkchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchan
*/

// }
