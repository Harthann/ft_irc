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

Socket * User::getSocket() const
{
	return this->self;
}

int 	User::getStatus() const
{
	return this->status;
}

void	User::setDatas(Commands cmd)
{
	if(cmd[0].compare("NICK") == 0)
		this->setNICK(cmd[1]);
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
}

User::~User()
{
}
