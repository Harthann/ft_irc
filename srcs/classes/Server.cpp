#include "Server.hpp"

	// if (host.host.sin_zero[0] == 'h')
	// 	proxy = true;
	// else
	// 	proxy = false;
Server::Server(int const &master_port, std::string &pass, host_info &host)
: users(), state(1), proxy(host.host.sin_zero[0] == 'h'), host(nullptr)
{
	this->master = new Socket(master_port);
	this->master->Listen();
	std::cout << "Constructing Server on port : " << master_port << "\n";
	std::cout << "Master socket on fd : " << this->master->getSocket() << std::endl;
	if (host.host.sin_zero[0] == 'h')
		this->host = new Socket(host);
	this->max_fd = this->master->getSocket();
	FD_ZERO(&this->readfds);
	FD_SET(this->master->getSocket(), &this->readfds);
	if (this->host) {
		FD_SET(this->host->getSocket(), &this->readfds);
		this->max_fd = std::max(this->master->getSocket(), this->host->getSocket());
	}
}

Server::Server(Server const &x)
{
	this->readfds = x.readfds;
	this->users = x.users;
	this->max_fd = x.max_fd;
}

Server	&Server::operator=(Server const &x)
{
	this->readfds = x.readfds;
	this->users = x.users;
	this->max_fd = x.max_fd;
	return (*this);
}

Server::~Server()
{
	for (std::vector<Socket*>::iterator it = users.begin(); it != users.end(); ++it)
		delete *it;
	delete master;
}

bool	Server::IsMaster(Socket*x)
{
	return (x == master);
}

Socket *Server::Select()
{
	int activity;

	activity = select(this->max_fd + 1, &this->readfds, NULL, NULL, NULL);
	if (activity < 0)
		throw Socket::SelectionError();
	for (size_t i = 0; i < users.size(); ++i) {
		if (FD_ISSET(this->users[i]->getSocket(), &this->readfds)) {
			std::cout << "Activity detected on client socket : " << users[i]->getSocket() << "\n";
			return this->users[i];
		}
	}
	if (FD_ISSET(this->host->getSocket(), &this->readfds))
		return (this->host);
	std::cout << "Activity detected on master socket : " << master->getSocket() << "\n";
	return master;
}

void	Server::add(Socket *x)
{
	std::string datas;
	std::string send_back("001 RPL_WELCOME  Welcome to the chat <user42>!<user42>@localhost\r\n");

	users.push_back(x);
	if (x->getSocket() > max_fd)
		max_fd = x->getSocket();
	x->Send(send_back.c_str());
	std::cout << datas << std::endl;
	FD_SET(x->getSocket(), &this->readfds);
}

void	Server::remove(Socket *x)
{
	struct sockaddr_in	tmp_addr_info;
	int					tmp_addr_len;

	getpeername(x->getSocket(), reinterpret_cast<struct sockaddr*>(&tmp_addr_info),
									reinterpret_cast<socklen_t*>(&tmp_addr_len));
	std::cout << "User left : " << inet_ntoa(tmp_addr_info.sin_addr);
	std::cout << " : " << ntohs(tmp_addr_info.sin_port) << std::endl;
	FD_CLR(x->getSocket(), &this->readfds);
	for (std::vector<Socket*>::iterator it = users.begin(); it != users.end(); ++it)
		if (*it == x)
		{
			std::cout << "Closing connection on socket : " << (*it)->getSocket() << std::endl;
			delete (*it);
			users.erase(it);
			break ;
		}
	this->max_fd = master->getSocket();
	for (std::vector<Socket*>::iterator it = users.begin(); it != users.end(); ++it)
		if ((*it)->getSocket() > this->max_fd)
			this->max_fd = (*it)->getSocket();
}

void	Server::update()
{
	FD_ZERO(&this->readfds);
	FD_SET(this->master->getSocket(), &this->readfds);
	max_fd = this->master->getSocket();
	FD_SET(this->host->getSocket(), &this->readfds);
	max_fd = std::max(max_fd, this->host->getSocket());

	for (std::vector<Socket*>::iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getSocket() > 0)
			FD_SET((*it)->getSocket(), &this->readfds);
		else {
			delete *it;
			it = users.erase(it);
		}
		if ((*it)->getSocket() > max_fd)
			max_fd = (*it)->getSocket();
	}
	for (size_t i = 0; i < users.size(); ++i) {
		if (users[i]->getSocket() > 0)
			FD_SET(users[i]->getSocket(), &this->readfds);
		if (users[i]->getSocket() > max_fd)
			max_fd = users[i]->getSocket();
	}
}

std::string		Server::IP() const
{
	return (this->master->IP());
}

void		Server::Stop()
{
	this->state = 0;
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