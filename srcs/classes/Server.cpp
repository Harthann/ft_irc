#include "Server.hpp"

Server::Server(int const &master_port, std::string pass)
: clients(), server_password(pass), state(1)
{
	std::cout << "Server password set as : " << server_password << std::endl;
	this->irc_log.open("irc_log", std::ios::out);
	std::cout << "Constructing Server on port : " << master_port << "\n";
	this->master = new Socket(master_port);
	std::cout << "Master socket created with fd : " << this->master->getSocket() << std::endl;
	this->master->Listen();
	std::cout << "Master now listening" << std::endl;
	this->max_fd = this->master->getSocket();
	FD_ZERO(&this->readfds);
	FD_SET(this->master->getSocket(), &this->readfds);
}

Server::Server(Server const &x)
{
	this->readfds = x.readfds;
	this->clients = x.clients;
	this->max_fd = x.max_fd;
}

//	Assignement constructor not defined yet
//	as it is probably useless or misleading on
//	server manipulation

// Server	&Server::operator=(Server const &x)
// {
// 	this->readfds = x.readfds;
// 	this->clients = x.clients;
// 	this->servers = x.servers;
// 	this->server_password = x.server_password;
// 	this->max_fd = x.max_fd;
// 	return (*this);
// }

Server::~Server()
{
	for (client_it it = clients.begin(); it != clients.end(); ++it)
		delete (*it);
	for (proxy_it it = servers.begin(); it != servers.end(); ++it)
		delete (*it).getSocketPtr();
	delete master;
}

//	Add a server to connect to base on info inside host_info structure
//	This server is added to the proxy list and is handle like one
//	We just need to redirect command to the server to propagate
//	the information and just indicate wich server as redirect the command
//	and the hopcount from the client.

void	Server::setHost(host_info &host)
{
	Socket *mem;
	try {
		std::string ret;
		mem = new Socket(host);
		std::cout << "Host socket connection is : ";
		std::cout << mem->getSocket() << std::endl;
		this->servers.push_back(mem);

		ret += "SERVER ftirc.test.42 1 :";
		if (mem->Connect())
			throw se::ConnectionFailed(mem->getInfo());
		ret.append(mem->getHostName());
		ret.append("Experimental server\r\n");
		mem->Send(ret);
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		throw e;
	}
}

//	Verify if a client is lready register as server or user

bool	Server::isRegister(Socket *client)
{
	for (proxy_it it = servers.begin(); it != servers.end(); ++it)
		if (client == (*it).getSocketPtr())
			return true;
	for (user_it it = users.begin(); it != users.end(); ++it)
		if (client == (*it).getSocketPtr())
			return true;
	return false;
}

//	Add a server to proxy list if he succeed identification

void	Server::setProxy(Commands &datas, Socket *client)
{
	int		hopcount;
	std::string name;

	if (datas.length() < 4) {
		client->Send("ERRROR :SERVER Wrong command format\r\n");
		return ;
	}
	for (Server::proxy_it it = servers.begin(); it != servers.end(); ++it)
		if ((*it).getSocketPtr() == client)
			return ;
	for (Server::client_it it = clients.begin(); it != clients.end(); ++it)
		if (*it == client) {
			std::cout << "Socket password : " << client->getPassword() << std::endl;
			if (client->getPassword() == this->server_password) {
				clients.erase(it);
			}
			else {
				client->Send("Connection refused");
				this->remove(*it);
				return ;
			}
			break ;
		}
	name = datas.getCmdParam(1);
	hopcount = utils::stoi(datas.getCmdParam(2));
	Proxy tmp(client, hopcount, name);
	this->servers.push_back(tmp);
	std::cout << "Added server to network" << std::endl;
}

//	Add a user to the users list if he succeed identification

void				Server::addUser(User &x)
{
	Socket *client = x.getSocketPtr();

	x.getSocketPtr()->Send("001 RPL_WELCOME " + x.getNick() + " Welcome to the server\r\n");
	for (client_it it = clients.begin(); it != clients.end(); ++it)
		if (*it == client) {
			std::cout << "Socket password : " << client->getPassword() << std::endl;
			if (client->getPassword() == this->server_password) {
				clients.erase(it);
				users.push_back(x);
			}
			else {
				client->Send("Connection refused");
				this->remove(*it);
				return ;
			}
			break ;
		}
	std::cout << "  ########### USER " << users.size() << " ############\n" << std::endl;
	x.displayinfo();
	std::cout << "  ###############################" << std::endl;
}

//	Detect activity on any clients connected to the server
//	Allow as well to identify if a server is ready to receive
//	datas by updating constantly writefds that contains
//	all writeable fds

Socket *Server::Select()
{
	int activity;

	activity = select(this->max_fd + 1, &this->readfds, &this->writefds, NULL, NULL);
	if (activity < 0)
		throw se::SelectFailed();
	for (Server::client_it it = this->clients.begin(); it != clients.end(); ++it) {
		if (this->readable(*it)) {
			this->irc_log << "Activity detected on client socket : " << (*it)->getSocket() << " for reading\n";
			return (*it);
		}
	}
	for (Server::proxy_it it = this->servers.begin(); it != servers.end(); ++it) {
		if (this->readable((*it).getSocketPtr())) {
			this->irc_log << "Activity detected on client socket : " << (*it).getSocket() << " for reading\n";
			return ((*it).getSocketPtr());
		}
	}
	for (Server::user_it it = this->users.begin(); it != users.end(); ++it) {
		if (this->readable((*it).getSocketPtr())) {
			this->irc_log << "Activity detected on client socket : " << (*it).getSocket() << " for reading\n";
			return ((*it).getSocketPtr());
		}
	}
	for (Server::client_it it = this->clients.begin(); it != clients.end(); ++it) {
		if (this->writeable(*it))
			return (*it);
	}
	if (this->readable(master))
		this->irc_log << "Activity detected on master socket : " << master->getSocket() << "\n";
	return master;
}

void	Server::add(Socket *x)
{
	std::string datas;

	clients.push_back(x);
	if (x->getSocket() > max_fd)
		max_fd = x->getSocket();
	FD_SET(x->getSocket(), &this->readfds);
	FD_SET(x->getSocket(), &this->writefds);
}

void	Server::removeSocket(Socket *x)
{
	FD_CLR(x->getSocket(), &this->readfds);
	for (client_it it = clients.begin(); it != clients.end(); ++it) {
		if ((*it) == x)
		{
			std::cout << "Closing connection on socket : " << (*it)->getSocket() << std::endl;
			delete (*it);
			clients.erase(it);
			return ;
		}
	}
	for (proxy_it it = servers.begin(); it != servers.end(); ++it) {
		if ((*it).getSocketPtr() == x) {
			std::cout << "Closing connection on socket : " << (*it).getSocket() << std::endl;
			delete (*it).getSocketPtr();
			servers.erase(it);
			return ;
		}
	}
	for (user_it it = users.begin(); it != users.end(); ++it)
	{
		if ((*it).getSocketPtr() == x) {
			std::cout << "Closing connection on socket : " << (*it).getSocket() << std::endl;
			delete (*it).getSocketPtr();
			users.erase(it);
			return ;
		}
	}
}

void	Server::remove(Socket *x)
{
	Addr	tmp;
	int		tmp_addr_len = tmp.addrlen();
	std::string	buffer;

	buffer = x->flush();
	this->irc_log << buffer << std::endl;
	std::cout << buffer <<std::endl;
	getpeername(x->getSocket(), tmp.as_sockaddr(),
									reinterpret_cast<socklen_t*>(&tmp_addr_len));
	std::cout << "User left : " << tmp.getIP();
	std::cout << " : " << tmp.getPort() << std::endl;
	removeSocket(x);
	this->update();
}

void	Server::fdSet(clients_vector &clients)
{
	Addr	tmp;
	int		tmp_addr_len = tmp.addrlen();

	for (client_it it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getSocket() > 0 && !timedOut(*it)) {
			FD_SET((*it)->getSocket(), &this->readfds);
			FD_SET((*it)->getSocket(), &this->writefds);
			if ((*it)->getSocket() > max_fd)
				max_fd = (*it)->getSocket();
		}
		else {
			getpeername((*it)->getSocket(), tmp.as_sockaddr(),
									reinterpret_cast<socklen_t*>(&tmp_addr_len));
			std::cout << "Connection timeout : " << tmp.getIP();
			std::cout << ":" << tmp.getPort() << std::endl;
			delete (*it);
			it = clients.erase(it);
			--it;
		}
	}
}

void	Server::fdSet(proxy_vector &)
{
	for (proxy_it it = servers.begin(); it != servers.end(); ++it) {
		if ((*it).getSocket() > 0) {
			FD_SET((*it).getSocket(), &this->readfds);
			FD_SET((*it).getSocket(), &this->writefds);
			if ((*it).getSocket() > max_fd)
				max_fd = (*it).getSocket();
		}
		else {
			delete ((*it).getSocketPtr());
			servers.erase(it);
		}
	}
}

void	Server::fdSet(user_vector &)
{
	for (user_it it = users.begin(); it != users.end(); ++it)
	{
		if ((*it).getSocket() > 0) {
			FD_SET((*it).getSocket(), &this->readfds);
			FD_SET((*it).getSocket(), &this->writefds);
			if ((*it).getSocket() > max_fd)
				max_fd = (*it).getSocket();
		}
		else {
			delete ((*it).getSocketPtr());
			users.erase(it);
		}
	}
}

//	Update fds that are connected to server
//	before using select

void	Server::update()
{
	FD_ZERO(&this->readfds);
	FD_ZERO(&this->writefds);
	FD_SET(this->master->getSocket(), &this->readfds);
	max_fd = this->master->getSocket();
	fdSet(this->clients);
	fdSet(this->servers);
	fdSet(this->users);
}

bool	Server::timedOut(Socket *x)
{
	if (!x)
		return true;
	if (!SET_TIMEOUT)
		return false;
	return (difftime(time(NULL), x->getTime()) > TIME_LIMIT);
}

void	Server::redirect(Commands &cmd, Socket *client)
{
	std::string datas = cmd.as_string();
	for (proxy_it it = servers.begin(); it != servers.end(); ++it)
	{
		// reedit commands part
		if ((*it).getSocketPtr() != client)
		{
			std::cout << "Redirect message to socket : ";
			std::cout << (*it).getSocket() << std::endl;
			(*it).getSocketPtr()->Send(datas);
		}
	}
}

std::vector<User>	&Server::getClients()
{
	return this->users;
}

std::vector<Channel>	& Server::getChannels()
{
	return this->channels;
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

bool		Server::readable(Socket *x) const
{
	return FD_ISSET(x->getSocket(), &this->readfds);
}

bool		Server::writeable(Socket *x) const
{
	return FD_ISSET(x->getSocket(), &this->writefds);
}

std::fstream	&Server::logStream()
{
	return (this->irc_log);
}

void			Server::addChannel(Channel &Ch)
{
	this->channels.push_back(Ch);
}

void			Server::logString(std::string to_log)
{
	this->irc_log << to_log << std::endl;
}
