#include "User.hpp"

User::User()
{
}

User::User(Socket *client, Commands cmd)
{
	this->self = client;
	this->status = 0;

	if(cmd[0].compare("PASS") == 0)
		this->setPASS(cmd[1]);
	else if(cmd[0].compare("NICK") == 0)
		this->setNICK(cmd[1]);
	else if(cmd[0].compare("USER") == 0)
		this->setUSER(cmd);
}


void	User::setPASS(std::string str)
{
	password.assign(str);
}

void	User::setNICK(std::string str)
{
	nickname.assign(str);
}

void	User::setUSER(Commands cmd)
{
	this->user = cmd[1];
	this->mode = cmd[2];
	std::string res = cmd[4].substr(1, cmd[4].length());
	realname.assign(res);
	this->status = 1;
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

std::string	User::getUser() const
{
	return this->user;
}

void	User::setDatas(Commands cmd)
{
	if(cmd[0].compare("NICK") == 0)
	{
		this->setNICK(cmd[1]);
	}
	else if(cmd[0].compare("USER") == 0)
		this->setUSER(cmd);
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
