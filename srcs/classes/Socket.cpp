#include "Socket.hpp"

Socket::Socket(int port, std::string , std::string IP)
{
	const int opt = 1;
	this->socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!this->socketfd)
		throw se::SocketFailed();
	addr_info.sin_family = AF_INET;
	addr_info.sin_port = htons(port);
	addr_info.sin_addr.s_addr = inet_addr(IP.c_str());
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	this->addr_len = sizeof(this->addr_info);
	if (this->Bind())
		throw se::InvalidBind();
}

Socket::Socket(host_info &host)
{
	const int opt = 1;
	this->socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr_info = host.host;
	std::cout << "Addrsss : " << inet_ntoa(addr_info.sin_addr) << std::endl;
	std::cout << "Port : " << ntohs(addr_info.sin_port) << std::endl;
	if (!this->socketfd)
		throw se::SocketFailed();
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	this->addr_len = sizeof(this->addr_info);
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
	this->socketfd = x.socketfd;
	this->addr_len = x.addr_len;
}

Socket	&Socket::operator=(Socket const &x)
{
	this->addr_info = x.addr_info;
	this->addr_len = x.addr_len;
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
	ret = bind(this->socketfd, reinterpret_cast<struct sockaddr*>(&this->addr_info), addr_len);
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
	ret = connect(this->socketfd, reinterpret_cast<struct sockaddr*>(&this->addr_info), addr_len);
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
	struct sockaddr_in	tmp_addr_info;
	int					tmp_addr_len;

	new_fd = accept(this->socketfd, reinterpret_cast<struct sockaddr*>(&tmp_addr_info),
									reinterpret_cast<socklen_t*>(&tmp_addr_len));
	if (new_fd < 0)
		throw se::InvalidAccept();
	std::cout << "New clients detected : " << inet_ntoa(tmp_addr_info.sin_addr);
	std::cout << " port : " << ntohs(tmp_addr_info.sin_port) << std::endl;
	return (new Socket(new_fd, tmp_addr_info, tmp_addr_len));
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
	return (inet_ntoa(this->addr_info.sin_addr));
}

sockaddr_in	Socket::getInfo() const {
	return this->addr_info;
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

/*
/join channelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelooochannelllllllkllkkkchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchannelchan
*/

// }