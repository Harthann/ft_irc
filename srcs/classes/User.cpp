#include "User.hpp"

User::User()
{
}

User::User(Socket *client, std::string datas)
{
	this->self = client;
	this->status = 0;
	char *str;

	str = (char *)datas.c_str();
	if(!strncmp(str, "PASS", 4))
		this->setPASS(str + 4);
	else if(!strncmp(str, "NICK", 4))
		this->setNICK(str + 4);
}

void	User::setPASS(char *str)
{
	int i = 0;

	while(str[i] >= 32)
		i++;
	password.assign(str, i);
}

void	User::setNICK(char *str)
{
	int i = 0;
	while(str[i] >= 32)
		i++;
	nickname.assign(str, i);
}

void	User::setUSER(std::string datas)
{
	char *str = (char *)datas.c_str();
	char *res;
	int i = 0;
	std::vector<char *> temp;
	char s[2] = " ";

	res = strtok(str, s);
	temp.push_back(res);
	while(res != NULL)
	{
		res = strtok(NULL, s);
		temp.push_back(res);
	}
	this->user = temp[1];
	this->mode = temp[2];
	res = temp[4];
	while(res[i] >= 32)
		i++;
	realname.assign(res + 1, i - 1);
}

Socket * User::getSocket() const
{
	return this->self;
}

int 	User::getStatus() const
{
	return this->status;
}

void	User::setDatas(std::string datas)
{
	char *str;

	str = (char *)datas.c_str();
	if(!strncmp(str, "PASS", 4))
		this->setPASS(str + 4);
	else if(!strncmp(str, "NICK", 4))
		this->setNICK(str + 4);
	else if(!strncmp(str, "USER", 4))
		this->setUSER(datas);

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
