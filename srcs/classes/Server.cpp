#include "Server.hpp"

Server::Server(int const &master_port, std::string pass)
:	pending_clients(), socket_list(), servers(), users(), unavailable_nicknames(),
	channels(), server_name(), server_message(), server_password(pass), state(1),
	timeout(0), last_ping(time(NULL))
{
	std::fstream	config_file;
	std::string		block;

	config_file.open("server.config", std::fstream::in);
	time(&this->launched_time);
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
	
	this->logString("Server password set as : " + server_password);
	this->logString("Server name is : " + this->server_name);
	this->logString("Server message is : " + this->server_message);
	this->logString("Constructing Server on port : " + utils::itos(master_port));
	this->logString("Timeout server set to : " + utils::itos(this->timeout));
	this->master = new Socket(master_port);
	this->logString("Master socket created with fd : " + utils::itos(this->master->getSocket()));
	this->master->Listen();
	this->logString("Master now listening");
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
	this->unavailable_nicknames = x.unavailable_nicknames;
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
	this->unavailable_nicknames = x.unavailable_nicknames;
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
			this->logString("Redirect message to socket : " + utils::itos((*it).getSocket()));
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
	if (this->timer() % PING_FREQUENCY == 0 && this->last_ping != time(NULL))
	{
		for (user_it it = users.begin(); it != users.end(); ++it)
		{
			(*it)->getSocketPtr()->bufferize(":" + this->server_name + " PING " + (*it)->getNickname());
			(*it)->getSocketPtr()->setPinged(time(NULL));
			// std::cout << "Pinged user : " << (*it)->getNickname() << std::endl;
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

void			Server::logString(std::string	to_log)
{
	this->irc_log << this->__header() << to_log;
	std::cout << this->__header() << to_log;
	if (*(to_log.end() - 1) != '\n')
	{
		this->irc_log << std::endl;
		std::cout << std::endl;
	}
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
int		Server::Select()
{
	int				activity;
	clients_vector	ret;
	struct timeval tv = {SELECT_TIMEOUT, 0};

	activity = select(this->max_fd + 1, &this->readfds, &this->writefds, NULL, &tv);
	return activity;
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
			this->logString("Connection timeout : " + tmp.strIP() + ":" + utils::itos(tmp.getPort()));
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
			this->logString("Connection timeout : " + tmp.strIP() + ":" + utils::itos(tmp.getPort()));
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
			this->logString("Connection timeout : " + tmp.strIP() + ":" + utils::itos(tmp.getPort()));
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
	this->logString("New clients detected from : " + x->IP());
	this->logString("Socket : " + utils::itos(x->getSocket()) + " has been added");
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
			this->logString("Socket password : " + client->getPassword());
			this->logString("Server password : " + this->server_password);
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
	x->getSocketPtr()->bufferize(":" + this->server_name + REPLY(RPL_WELCOME) + x->getNickname() + " Welcome to the server\r\n", MSG_TYPE);
	this->logString("############### USER " + utils::itos(users.size()) + " ##################");
	this->logString("#\t\t" + x->getNickname() + "\t\t\t#");
	this->logString("#########################################");
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
		this->logString("Host socket connection is : " + utils::itos(mem->getSocket()));
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
			client->bufferize(":" + this->server_name + REPLY(ERR_ALREADYREGISTRED) + ":You may not reregister");
			return ;
		}
	}
	for (Server::client_it it = pending_clients.begin(); it != pending_clients.end(); ++it)
	{
		if (*it == client) {
			this->logString("Socket password : " + client->getPassword());
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
	this->logString("Added server to network");
}

void			Server::addChannel(Channel *Ch)
{
	this->channels.push_back(Ch);
}

void			Server::addUnavailableNickname(std::string nick) {
	this->unavailable_nicknames.push_back(nick);
}

void	Server::remove(Socket *x)
{
	Addr	tmp;
	int		tmp_addr_len = tmp.addrlen();
	std::string	buffer;

	buffer = x->flush();
	this->irc_log << buffer << std::endl;
	this->logString(buffer);
	getpeername(x->getSocket(), tmp.as_sockaddr(),
				reinterpret_cast<socklen_t*>(&tmp_addr_len));
	this->logString("User left : " + tmp.strIP() + " : " + utils::itos(tmp.getPort()));
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
			this->logString("Closing connection on socket : " + utils::itos((*it)->getSocket()));
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
			this->logString("Server " + utils::itos((*it).getSocket()) + " left");
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

User 	*Server::getUserByName(std::string name)
{
	for (user_it it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getNickname() == name)
			return (*it);
	}
	return (NULL);
}

std::vector<Channel *>	& Server::getChannels()
{
	return this->channels;
}

std::string				&Server::getServerName() {
	return (this->server_name);
}

std::vector<Socket *>	&Server::getSocketList()
{
	return this->socket_list;
}

std::vector<std::string>	&Server::getUnavailableNicknames() {
	return (this->unavailable_nicknames);
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

time_t		Server::timer() const
{
	return difftime(time(NULL), this->launched_time);
}

bool		Server::IsUserOnServer(std::string nickname) {
	for (user_vector::iterator it = this->getClients().begin(); it != this->getClients().end(); ++it) {
		if ((*it)->getNickname() == nickname)
			return (true);
	}
	return (false);
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


/****************************************************************/
/*						Private function						*/
/****************************************************************/

std::string Server::__header()
{
	std::string ret;

	ret += '[';
	ret += utils::itos(this->timer());
	ret += "]\t\t";
	return ret;
}
