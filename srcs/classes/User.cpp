#include "User.hpp"

User::User()
{
}

User::User(Socket *client, std::string datas)
{
	this->self = client;
	this->status = 0;
	char *str;

	str = datas.c_str();
	if(!strncmp(str, "PASS", 4))
		this->setPASS(str + 4);
	else if(!strncmp(str, "NICK", 4))
		this->setNICK(str + 4);
}

Socket * User::getSocket() const
{
	return this->self;
}

int 	User::getStatus() const
{
	return this->status;
}

User::~User()
{
}
