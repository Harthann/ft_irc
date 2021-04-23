#include "Socket.hpp"

Socket::Socket(int port, std::string , std::string IP)
: addr()
{
	time(&this->timestamp);
	const int opt = 1;
	this->socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!this->socketfd)
		throw se::SocketFailed();
	this->addr.setPort(port);
	this->addr.setIP(IP.c_str());
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	if (this->Bind())
		throw se::InvalidBind();
}

Socket::Socket(host_info &host)
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
}

Socket::Socket(int fd, Addr ad, int length)
{
	this->socketfd = fd;
	this->addr = ad;
}

Socket::Socket(Socket const &x)
{
	this->addr = x.addr;
	this->socketfd = x.socketfd;
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

std::string Socket::Receive()
{
	std::string	ret;
	char		buffer[513];
	int			readed;

	do {
		readed = read(this->socketfd, buffer, 512);
		if (readed > 0)
			buffer[readed] = '\0';
		else
			return ("");
		ret += buffer;
	} while (readed && *(ret.end() - 1) != '\n' && *(ret.end() - 2) != '\r');
	return (ret);
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
	return ("");
	// struct hostent *tmp;
	// char buff[256];
	
	// // getnameinfo(reinterpret_cast<const sockaddr *>(&this->addr_info), sizeof(this->addr_info), buff, 256, NULL, 0, 0);
	// // return (buff);
	// tmp = gethostbyaddr(reinterpret_cast<const struct in_addr*>(&this->addr_info), sizeof(struct in_addr), AF_INET);
	// if (!tmp)
	// 	return ("");
	// return tmp->h_name;
}

time_t		Socket::getTime() const
{
	return (this->timestamp);
}

/*
/join channelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelooochannelllllllkllkkkchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchan
*/

// }