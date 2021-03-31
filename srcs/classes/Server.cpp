#include "Server.hpp"

Server::Server(int const &master_port, std::string pass)
: clients(), host(nullptr), server_password(pass), state(1), proxy(0)
{
	std::cout << "Constructing Server on port : " << master_port << "\n";
	this->master = new Socket(master_port);
	std::cout << "Master socket created with fd : " << this->master->getSocket() << std::endl;
	this->master->Listen();
	std::cout << "Master now listening" << std::endl;
	this->max_fd = this->master->getSocket();
	FD_ZERO(&this->readfds);
	FD_SET(this->master->getSocket(), &this->readfds);

	std::cout << "Master pointer adress : " <<  this->master << std::endl;
}

Server::Server(Server const &x)
{
	this->readfds = x.readfds;
	this->clients = x.clients;
	this->max_fd = x.max_fd;
}

Server	&Server::operator=(Server const &x)
{
	this->readfds = x.readfds;
	this->clients = x.clients;
	this->max_fd = x.max_fd;
	return (*this);
}

Server::~Server()
{
	for (client_it it = clients.begin(); it != clients.end(); ++it)
		delete (*it).first;
	delete master;
}

void	Server::setHost(host_info &host)
{
	try {
		std::string ret("SERVER ftirc.test.42 1 :");
		this->host = new Socket(host);
		std::cout << "Host socket adress is : " << this->host << std::endl;
		this->proxy = 1;
		if (this->host->Connect())
			throw se::ConnectionFailed(this->host->getInfo());
		ret.append(this->host->getHostName());
		ret.append("  Experimental server\r\n");
		this->host->Send(ret);
	} catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		throw e;
	}
}

Socket *Server::Select()
{
	int activity;

	activity = select(this->max_fd + 1, &this->readfds, NULL, NULL, NULL);
	if (activity < 0)
		throw se::SelectFailed();
	for (Server::client_it it = this->clients.begin(); it != clients.end(); ++it) {
		if (FD_ISSET((*it).first->getSocket(), &this->readfds)) {
			std::cout << "Activity detected on client socket : " << (*it).first->getSocket() << "\n";
			return (*it).first;
		}

	}
	if (this->host && FD_ISSET(this->host->getSocket(), &this->readfds))
		return (this->host);
	std::cout << "Activity detected on master socket : " << master->getSocket() << "\n";
	return master;
}

void	Server::add(Socket *x)
{
	std::string datas;

	clients[x] = 0;
	if (x->getSocket() > max_fd)
		max_fd = x->getSocket();
	FD_SET(x->getSocket(), &this->readfds);
}

void	Server::remove(Socket *x)
{
	struct sockaddr_in	tmp_addr_info;
	int					tmp_addr_len;

	if (x == this->host)
		throw se::LostConnection(this->host->getInfo());
	getpeername(x->getSocket(), reinterpret_cast<struct sockaddr*>(&tmp_addr_info),
									reinterpret_cast<socklen_t*>(&tmp_addr_len));
	std::cout << "User left : " << inet_ntoa(tmp_addr_info.sin_addr);
	std::cout << " : " << ntohs(tmp_addr_info.sin_port) << std::endl;
	FD_CLR(x->getSocket(), &this->readfds);
	for (client_it it = clients.begin(); it != clients.end(); ++it)
		if ((*it).first == x)
		{
			std::cout << "Closing connection on socket : " << (*it).first->getSocket() << std::endl;
			delete (*it).first;
			clients.erase(it);
			break ;
		}
	this->max_fd = master->getSocket();
	for (client_it it = clients.begin(); it != clients.end(); ++it)
		if ((*it).first->getSocket() > this->max_fd)
			this->max_fd = (*it).first->getSocket();
}

void	Server::update()
{
	FD_ZERO(&this->readfds);
	FD_SET(this->master->getSocket(), &this->readfds);
	max_fd = this->master->getSocket();
	if (this->host) {
		FD_SET(this->host->getSocket(), &this->readfds);
		max_fd = std::max(max_fd, this->host->getSocket());
	}
	for (client_it it = clients.begin(); it != clients.end(); ++it) {
		if ((*it).first->getSocket() > 0)
			FD_SET((*it).first->getSocket(), &this->readfds);
		else {
			delete (*it).first;
			clients.erase(it);
		}
		if ((*it).first->getSocket() > max_fd)
			max_fd = (*it).first->getSocket();
	}
}

char	&Server::setClientType(Socket *x)
{
	if (this->clients.find(x) == this->clients.end())
		throw "Socket not found";
	return (*this->clients.find(x)).second;
}

std::string		Server::IP() const
{
	return (this->master->IP());
}

void		Server::Stop()
{
	this->state = 0;
}

bool	Server::IsMaster(Socket*x)
{
	return (x == master);
}

bool		Server::IsRunning() const
{
	return this->state;
}

bool		Server::IsProxy() const
{
	return this->proxy;
}

bool		Server::IsHost(Socket *x) const
{
	return x == this->host;
}