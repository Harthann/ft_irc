#include "User.hpp"

User::User()
{
}

User::User(Socket *client, Commands cmd, std::string server_name)
: flags(0), away_message()
{
	this->self = client;
	this->status = 0;

	if(cmd.length() > 1)
	{
		if(cmd[0].compare("PASS") == 0)
			this->setPASS(cmd[1]);
		else if(cmd[0].compare("NICK") == 0)
			this->setNICK(cmd[1]);
		else if(cmd[0].compare("USER") == 0)
			this->setUSER(cmd, server_name);
	}
	else
	{
		std::string msg = ":" + server_name + REPLY(ERR_NEEDMOREPARAMS) + cmd[0] + " :Not enough parameters\n";
		this->self->bufferize(msg);
	}

}


void	User::setPASS(std::string str)
{
	password.assign(str);
}

void	User::setNICK(std::string str)
{
	nickname.assign(str);
}

void	User::setAwayMessage(std::string str)
{
	this->away_message = str;
}

void	User::setUSER(Commands cmd, std::string server_name)
{
	if (cmd.length() > 3)
	{
		this->user = cmd[1];
		this->mode = cmd[2];
		std::string res = cmd[4].substr(1, cmd[4].length());
		realname.assign(res);
		this->status = 1;
	}
	else
	{
		std::string msg = ":" + server_name + REPLY(ERR_NEEDMOREPARAMS) + cmd[0] + " :Not enough parameters\n";
		this->self->bufferize(msg);
	}
}

Socket * User::getSocketPtr() const
{
	return this->self;
}

int 	User::getStatus() const
{
	return this->status;
}

std::string	User::getNickname() const
{
	return this->nickname;
}

const std::string	&User::getAwayMessage() const
{
	return this->away_message;
}

std::string	User::getUser() const
{
	return this->user;
}

std::vector<Channel *>	&User::getChannels() {
	return (this->current_channel);
}

void	User::setDatas(Commands cmd, std::string server_name)
{
	if (cmd.length() > 1)
	{
		if(cmd[0].compare("NICK") == 0)
		{
			this->setNICK(cmd[1]);
		}
		else if(cmd[0].compare("USER") == 0)
			this->setUSER(cmd, server_name);
	}
	else
	{
		std::string msg = ":" + server_name + REPLY(ERR_NEEDMOREPARAMS) + cmd[0] + " :Not enough parameters\n";
		this->self->bufferize(msg);
	}
}

std::string	&User::getRealName() {
	return (this->realname);
}

bool		User::haveCommonChanel(User *user) {
	for (std::vector<Channel *>::iterator it = this->getChannels().begin(); it != this->getChannels().end(); ++it) {
		for (std::vector<Channel *>::iterator ite = user->getChannels().begin(); ite != user->getChannels().end(); ++ite) {
			if ((*it) == (*ite))
				return (true);
		}
	}
	return (false);
}

void	User::displayinfo()
{
	std::cout << "nickname : " << nickname << std::endl;
	std::cout << "password : " << password << std::endl;
	std::cout << "user : " << user << std::endl;
	std::cout << "mode : " << mode << std::endl;
	std::cout << "status : " << status << std::endl;
	std::cout << "realname : " << realname << std::endl;
	std::cout << "Socketptr : " << this->getSocketPtr() << std::endl;
	std::cout << "Socket : " << this->getSocket() << std::endl;
}

void	User::ActiveChannel(Channel *ch)
{
	this->current_channel.push_back(ch);
}

int		User::getSocket() const
{
	return (this->self->getSocket());
}

void	User::partChannel(std::string ch)
{
	Channel *temp;

	for(unsigned int i = 0; i < current_channel.size(); ++i)
	{
		if (current_channel[i]->getName() == ch)
		{
			temp = current_channel[i];
			temp->part(this->getSocketPtr());
			current_channel.erase(current_channel.begin() + i);
		}
	}
}

void	User::partChannels()
{
	for(unsigned int i = 0; i < current_channel.size(); ++i)
	{
		current_channel[i]->part(this->getSocketPtr());
		current_channel.erase(current_channel.begin() + i);
	}
}

std::string		User::getID() const {
	std::string ret;

	ret.append(":" + this->getNickname() +
				"!~" + this->getNickname() +
				"@" + this->getSocketPtr()->IP());
	return (ret);
}

/****************************************************************/
/*							Flags								*/
/****************************************************************/

void	User::enableFlag(char mask) {
	// this->flags = this->flags | mask;
	this->flags |= mask;
}

void	User::disableFlag(char mask) {
	// this->flags = this->flags ^ mask;
	this->flags ^= mask;
}

bool	User::flagIsSet(char mask) {
	return (this->flags & mask);
}

bool	User::operator == (User *u2)
{
	if (this->nickname.compare(u2->getNickname()) == 0)
		return 0;
	else
		return 1;
}

User::~User()
{
}
