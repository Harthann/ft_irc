#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string Name, User &C_operator)
{
	this->name = Name;
	this->channel_operator = &C_operator;
	this->active_users.push_back(C_operator);
	std::string msg = "Welcome to the newly created channel " + Name + "\n";
	send(C_operator.getSocket()->getSocket(), msg.c_str(), msg.length(), 0);
}

void				Channel::addUser(User &user)
{
	this->active_users.push_back(user);
	std::string msg = "Welcome to  " + this->name + "\n";
	send(user.getSocket()->getSocket(), msg.c_str(), msg.length(), 0);
}


std::string			Channel::getName()
{
	return this->name;
}

Channel::~Channel()
{

}
