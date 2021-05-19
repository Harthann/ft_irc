#include "Server.hpp"

Server::Server(int const &master_port, std::string pass)
:	pending_clients(), socket_list(), servers(), users(), channels(),
	server_name(), server_message(), server_password(pass), state(1),
	timeout(0), last_ping(time(NULL))
{
	std::fstream	config_file;
	std::string		block;

	config_file.open("server.config", std::fstream::in);
	if (config_file.fail())
		throw ErrorOpeningFile();
	do
	{
		try {
			block = __extract_block(config_file);
		} catch (se::ServerException &e) {
			throw e;
		}
		if (!block.compare(0, block.find(' '), "me")) {
			this->server_name = __process_block(block, "name");
			this->server_message = __process_block(block, "info");
		}
		else if (!block.compare(0, block.find(' '), "log"))
			this->irc_log.open(__process_block(block, "file").c_str(), std::ios::out);
		else if (!block.compare(0, block.find(' '), "set"))
			this->timeout = utils::stoi(__process_block(block, "timeout"));
	} while (!config_file.eof());
	
	std::cout << "Server password set as : " << server_password << std::endl;
	std::cout << "Server name is : " << this->server_name << std::endl;
	std::cout << "Server message is : " << this->server_message << std::endl;
	std::cout << "Constructing Server on port : " << master_port << "\n";
	std::cout << "Timeout server set to : " << this->timeout << std::endl;
	this->master = new Socket(master_port);
	std::cout << "Master socket created with fd : " << this->master->getSocket() << std::endl;
	this->master->Listen();
	std::cout << "Master now listening" << std::endl;
	this->max_fd = this->master->getSocket();
	FD_ZERO(&this->readfds);
	FD_ZERO(&this->writefds);
	FD_SET(this->master->getSocket(), &this->readfds);
	this->socket_list.push_back(this->master);
}

Server::Server(Server const &x)
{
	this->readfds = x.readfds;
	this->pending_clients = x.pending_clients;
	this->socket_list = x.socket_list;
	this->max_fd = x.max_fd;
}

/*
**	Assignement constructor not defined yet
**	as it is probably useless or misleading on
**	server manipulation
*/
Server	&Server::operator=(Server const &x)
{
	this->readfds = x.readfds;
	// this->clients = x.clients;
	this->servers = x.servers;
	this->server_password = x.server_password;
	this->max_fd = x.max_fd;
	return (*this);
}

Server::~Server()
{
	for (client_it it = socket_list.begin(); it != socket_list.end(); ++it)
		delete (*it);
}

void		Server::Stop()
{
	this->state = 0;
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
			(*it).getSocketPtr()->bufferize(datas);
		}
	}
}

void	Server::flushClient()
{
	for (client_it it = pending_clients.begin(); it != pending_clients.end(); ++it)
		if (writeable(*it))
			(*it)->flushWrite();
	for (proxy_it it = servers.begin(); it != servers.end(); ++it)
		if (writeable((*it).getSocketPtr()))
			(*it).getSocketPtr()->flushWrite();

}

void	Server::ping()
{
	if (difftime(time(NULL), this->last_ping) >= 30)
	{
		for (user_it it = users.begin(); it != users.end(); ++it)
		{
			(*it)->getSocketPtr()->bufferize(":" + this->server_name + " PING " + (*it)->getNickname());
			(*it)->getSocketPtr()->setPinged(time(NULL));
			std::cout << "Pinged user : " << (*it)->getNickname() << std::endl;
		}
		time(&this->last_ping);
	}
}

/****************************************************************/
/*					logs function to keep a trace of events		*/
/****************************************************************/
std::fstream	&Server::logStream()
{
	return (this->irc_log);
}

void			Server::logString(std::string to_log)
{
	this->irc_log << to_log << std::endl;
}

/****************************************************************/
/*				Select linked function							*/
/****************************************************************/
/*
**	Detect activity on any clients connected to the server
**	Allow as well to identify if a server is ready to receive
**	datas by updating constantly writefds that contains
**	all writeable fds
*/
Server::clients_vector &Server::Select()
{
	int				activity;
	clients_vector	ret;
	struct timeval tv = {10, 0};

	activity = select(this->max_fd + 1, &this->readfds, &this->writefds, NULL, &tv);
	if (activity < 0)
		throw se::SelectFailed();
	return this->socket_list;
}

void	Server::update()
{
	Addr	tmp;
	int		tmp_addr_len = tmp.addrlen();
	
	FD_ZERO(&this->readfds);
	FD_ZERO(&this->writefds);
	FD_SET(this->master->getSocket(), &this->readfds);
	max_fd = this->master->getSocket();

	for (user_it it = users.begin(); it != users.end(); ++it)
	{
		if (timedOut((*it)->getSocketPtr())) {
			getpeername((*it)->getSocket(), tmp.as_sockaddr(),
									reinterpret_cast<socklen_t*>(&tmp_addr_len));
			std::cout << "Connection timeout : " << tmp.getIP();
			std::cout << ":" << tmp.getPort() << std::endl;
			// delete ((*it)->getSocketPtr());
			it = users.erase(it);
			this->removeSocket((*it)->getSocketPtr());
			--it;
		}
	}
	fdSet(socket_list);
	// fdSet(pending_clients);
	// fdSet(this->users);
}

void	Server::fdSet(clients_vector &clients)
{
	Addr	tmp;
	int		tmp_addr_len = tmp.addrlen();

	for (client_it it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getSocket() > 0) {
			FD_SET((*it)->getSocket(), &this->readfds);
			if (!(*it)->bufferEmpty())
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
	Addr	tmp;
	int		tmp_addr_len = tmp.addrlen();

	for (user_it it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getSocket() > 0 && !timedOut((*it)->getSocketPtr())) {
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
			// delete ((*it)->getSocketPtr());
			it = users.erase(it);
			--it;
		}
	}
}

bool		Server::readable(Socket *x) const
{
	return FD_ISSET(x->getSocket(), &this->readfds);
}

bool		Server::writeable(Socket *x) const
{
	return FD_ISSET(x->getSocket(), &this->writefds);
}

/****************************************************************/
/*			Controls Socket flows inside server					*/
/****************************************************************/
void	Server::add(Socket *x)
{
	std::string datas;

	pending_clients.push_back(x);
	socket_list.push_back(x);
	std::cout << "Socket : " << x->getSocket() << " has been added" << std::endl;
	if (x->getSocket() > max_fd)
		max_fd = x->getSocket();
	FD_SET(x->getSocket(), &this->readfds);
	FD_SET(x->getSocket(), &this->writefds);
}

void				Server::addUser(User *x)
{
	Socket *client = x->getSocketPtr();

	for (proxy_it it = servers.begin(); it != servers.end(); ++it)
	{
		if (x->getSocketPtr() == (*it).getSocketPtr()) {
			if (client->getPassword() == this->server_password)
				users.push_back(x);
			else {
				client->bufferize("Connection refused");
				return ;
			}
		}
	}
	for (client_it it = pending_clients.begin(); it != pending_clients.end(); ++it)
	{
		if (*it == client) {
			std::cout << "Socket password : " << client->getPassword() << std::endl;
			std::cout << "Server password : " << this->server_password << std::endl;
			if (client->getPassword() == this->server_password) {
				pending_clients.erase(it);
				users.push_back(x);
			}
			else {
				client->bufferize("Connection refused");
				this->remove(*it);
				return ;
			}
			break ;
		}
	}
	x->getSocketPtr()->bufferize("001 RPL_WELCOME " + x->getNickname() + " Welcome to the server\r\n", MSG_TYPE);
	std::cout << "  ########### USER " << users.size() << " ############\n" << std::endl;
	x->displayinfo();
	std::cout << "  ###############################" << std::endl;
}

/*
**	Add a server to connect to base on info inside host_info structure
**	This server is added to the proxy list and is handle like one
**	We just need to redirect command to the sergver to propagate
**	the information and just indicate wich server as redirect the command
**	and the hopcount from the client.
*/
void	Server::setHost(host_info &host)
{
	Socket *mem;
	try {
		std::string ret;
		mem = new Socket(host);
		std::cout << "Host socket connection is : ";
		std::cout << mem->getSocket() << std::endl;
		this->servers.push_back(mem);
		this->socket_list.push_back(mem);

		ret += "SERVER " + this->server_name + " 1 :";
		if (mem->Connect())
			throw se::ConnectionFailed(mem->getInfo());
		ret.append(mem->getHostName());
		ret.append("Experimental server\r\n");
		mem->bufferize(ret);
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		throw e;
	}
}

/*	Add a server to proxy list if he succeed identification */
void	Server::setProxy(Commands &datas, Socket *client)
{
	int		hopcount;
	std::string name;

	if (datas.length() < 4) {
		client->bufferize("ERRROR :SERVER Wrong command format\r\n");
		return ;
	}
	for (Server::proxy_it it = servers.begin(); it != servers.end(); ++it)
	{
		if ((*it).getName() == datas[1])
		{
			client->bufferize("462	ERR_ALREADYREGISTRED :You may not reregister");
			return ;
		}
	}
	for (Server::client_it it = pending_clients.begin(); it != pending_clients.end(); ++it)
	{
		if (*it == client) {
			std::cout << "Socket password : " << client->getPassword() << std::endl;
			if (client->getPassword() == this->server_password) {
				pending_clients.erase(it);
			}
			else {
				client->bufferize("Connection refused");
				this->remove(*it);
				return ;
			}
			break ;
		}
	}
	name = datas.getCmdParam(1);
	hopcount = utils::stoi(datas.getCmdParam(2));
	Proxy tmp(client, hopcount, name);
	this->servers.push_back(tmp);
	std::cout << "Added server to network" << std::endl;
}

void			Server::addChannel(Channel *Ch)
{
	this->channels.push_back(Ch);
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

void	Server::removeSocket(Socket *x)
{
	FD_CLR(x->getSocket(), &this->readfds);
	for (client_it it = pending_clients.begin(); it != pending_clients.end(); ++it) {
		if ((*it) == x)
		{
			pending_clients.erase(it);
			break ;
		}
	}
	for (proxy_it it = servers.begin(); it != servers.end(); ++it) {
		if ((*it).getSocketPtr() == x) {
			servers.erase(it);
			break ;
		}
	}
	for (user_it it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getSocketPtr() == x) {
			users.erase(it);
			break ;
		}
	}
	for (client_it it = socket_list.begin(); it != socket_list.end(); ++it) {
		if ((*it) == x)
		{
			std::cout << "Closing connection on socket : " << (*it)->getSocket() << std::endl;
			delete (*it);
			socket_list.erase(it);
			return ;
		}
	}
}

void			Server::delete_user(User *user, std::string msg1)
{
	std::string msg;
	for(unsigned int i = 0; i < users.size(); ++i)
	{
		if(user->getSocketPtr() == users[i]->getSocketPtr())
		{
			msg = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1 QUIT :" + msg1 + "\n";
			user->getSocketPtr()->bufferize(msg);
			users.erase(users.begin() + i);
			break;
		}
	}
}

void	Server::removeServer(Socket *x)
{
	for (proxy_it it = servers.begin(); it != servers.end(); ++it)
	{
		//	Will need to change condition to use server name instead
		if (x == (*it).getSocketPtr()) {
			std::cout << "Server " << (*it).getSocket() << " left" << std::endl;
			it = servers.erase(it);
		}
	}
}

/****************************************************************/
/*							Getters								*/
/****************************************************************/

std::vector<User *>	&Server::getClients()
{
	return this->users;
}

User 	&Server::getUserByName(std::string name)
{
	for (user_it it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getNickname() == name)
			return (**it);
	}
}

std::vector<Channel *>	& Server::getChannels()
{
	return this->channels;
}

std::string				&Server::getServerName() {
	return (this->server_name);
}

/****************************************************************/
/*					Information function						*/
/****************************************************************/
std::string		Server::IP() const
{
	return (this->master->IP());
}

bool	Server::IsMaster(Socket*x)
{
	return (x == master);
}

bool		Server::IsRunning() const
{
	return this->state;
}

bool	Server::isRegister(Socket *client)
{
	for (proxy_it it = servers.begin(); it != servers.end(); ++it)
		if (client == (*it).getSocketPtr())
			return true;
	for (user_it it = users.begin(); it != users.end(); ++it)
		if (client == (*it)->getSocketPtr())
			return true;
	return false;
}

bool	Server::timedOut(Socket *x)
{
	if (!x)
		return true;
	if (!this->timeout || !x->getPingedTime())
		return false;
	return (difftime(time(NULL), x->getPingedTime()) > this->timeout);
}

void		Server::checkChannels()
{
	Channel *temp;
	for(unsigned int i = 0; i < channels.size(); ++i)
	{
		if (channels[i]->NumberOfUsers() == 0)
		{
			temp = channels[i];
			channels.erase(channels.begin() + i);
			delete temp;
		}
	}
}


/****************************************************************/
/*						Block parser							*/
/****************************************************************/
std::string		Server::__extract_block(std::fstream &fs)
{
	std::string ret;
	std::string buffer;

	do
	{
		getline(fs, buffer);
	} while (buffer.empty() && !fs.eof());
	if (buffer.empty())
		return buffer;
	ret += buffer;
	do
	{
		getline(fs, buffer);
		if ((buffer[buffer.size() - 1] != '{' && buffer[buffer.size() - 1] != '}' && buffer[buffer.size() - 1] != ';')
			|| (buffer[buffer.size()] == '{' && buffer[buffer.size() - 2] != ' '))
			throw (WrongConfigFileFormat());
 		ret += buffer;
	} while (buffer[buffer.size() - 1] != '}' && !fs.eof());
	return ret;
}

std::string		Server::__process_block(std::string &str, std::string keyword)
{
	size_t			pos;
	size_t			second_pos;
	std::string		ret;

	pos = str.find('{');
	while (pos != std::string::npos && str[pos] != '}')
	{
		while (!std::isalpha(str[pos]))
			++pos;
		second_pos = str.find(' ', pos);
		// if (keyword == "timeout")
		// 	std::cout << "Timeout found" << std::endl;
		if (!str.compare(pos, second_pos - pos, keyword))
		{
			pos = str.find('"', pos);
			second_pos = str.find('"', pos + 1);
			ret = str.substr(pos + 1, second_pos - pos - 1);
			break ;
		}
		pos = str.find(';', pos) + 1;
	}
	return ret;
}
