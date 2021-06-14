#include "Socket.hpp"
#include <fcntl.h>

Socket::Socket(int port, std::string , std::string IP)
: addr(), ping_ts(0), password(), recv_buffer(), cmd_buffer(),  writable(false)
{
	time(&this->timestamp);
	const int opt = 1;
	this->socketfd = socket(AF_INET & AF_INET6, SOCK_STREAM, IPPROTO_TCP);
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
: ping_ts(0), password(), recv_buffer(), cmd_buffer(),  writable(false)
{
	const int opt = 1;
	time(&this->timestamp);
	this->socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!this->socketfd)
		throw se::SocketFailed();
	this->addr = host.host;
	std::cout << "Address : " << this->addr.getIP() << std::endl;
	std::cout << "Port : " << this->addr.getPort() << std::endl;
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  	setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	setsockopt(this->socketfd, SOL_SOCKET, SO_TIMESTAMP, &opt, sizeof(opt));
}

Socket::Socket(int fd, Addr ad)
: ping_ts(0), password(), recv_buffer(), cmd_buffer(),  writable(false)
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


/****************************************************************/
/*					Socket action/role control					*/
/****************************************************************/
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
	return (new Socket(new_fd, addr));
}

/****************************************************************/
/*							Setters								*/
/****************************************************************/
void			Socket::setPassword(std::string &x)
{
	this->password = x;
}

void			Socket::setPinged(time_t ts)
{
	this->ping_ts = ts;
}

/****************************************************************/
/*					Getters and Info							*/
/****************************************************************/

const std::string		&Socket::getPassword() const
{
	return (this->password);
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

const time_t		&Socket::getPingedTime() const
{
	return this->ping_ts;
}

Addr	Socket::getInfo() const {
	return this->addr;
}

const int	&Socket::getSocket() const {
	return (this->socketfd);
}



std::string	Socket::getHostName() const
{
	//	Probably not necessary and actually not working well
	//	This function will eventually be deleted if no solution found
	//	The goal is found host name base on ip address
	//	Ex : ip = 127.0.0.1 will give "localhost" as return value
	return ("");
}

std::string		Socket::IP() const
{
	return (this->addr.getIP());
}

bool		Socket::bufferEmpty() const
{
	return (this->cmd_buffer.empty());
}

/****************************************************************/
/*					Read/write buffers control					*/
/****************************************************************/
void	Socket::Send(std::string message, int type)
{
	send(this->socketfd, message.c_str(), message.length(), type * MSG_CONFIRM);
}

void	Socket::Confirm(std::string message)
{
	send(this->socketfd, message.c_str(), message.length(), MSG_CONFIRM);
}

/*	Unefficient version that work just fine for the moment */
std::string Socket::Receive()
{
	std::string	ret;
	char		buffer;
	int			readed;

	do {
		readed = read(this->socketfd, &buffer, 1);
		if (readed > 0)
			ret += buffer;
	} while (readed && *(ret.end() - 1) != '\n' && *(ret.end() - 2) != '\r');
	return (ret);
}

/* Efficient version in theory but still don't work correctly
std::string Socket::Receive()
{
	std::string	ret;
	char		buffer[513];
	int			readed;
	size_t			pos;

	readed = read(this->socketfd, buffer, 512);
	buffer[readed] = '\0';
	if (readed > 0)
		this->recv_buffer += buffer;
	pos = this->recv_buffer.find("\r\n", 0);
	if (pos == std::string::npos)
		pos = this->recv_buffer.size() - 2;
	pos += 2;
	ret = this->recv_buffer.substr(0, pos);
	this->recv_buffer.erase(0, pos);
	return (ret);
}
*/

void			Socket::bufferize(Commands &cmd, int type)
{
	cmd.setType(type);
	cmd_buffer.push_back(cmd);
}

void			Socket::bufferize(std::string cmd, int type)
{
	cmd_buffer.push_back(Commands(cmd, type));
}

void			Socket::flushWrite()
{
	if (!cmd_buffer.empty())
	{
		this->Send(cmd_buffer[0].as_string(), cmd_buffer[0].getType());
		cmd_buffer.erase(cmd_buffer.begin());
	}
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

// }
