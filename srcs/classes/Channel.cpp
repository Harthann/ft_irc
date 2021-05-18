#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string Name, User *C_operator, std::string server_name) : name(Name), topic(""), server_name(server_name)
{
	this->channel_operator = C_operator;
	this->active_users.push_back(C_operator);
	this->list_all_users.push_back("@" + C_operator->getNickname());
	std::string temp = ":" + C_operator->getUser() + "!~" + C_operator->getUser() + "@127.0.0.1";
	std::string msg = temp + " JOIN :"+ Name + "\n";
	C_operator->getSocketPtr()->bufferize(msg);
	if (this->topic != "")
	{
		msg = ":" + server_name + " 332 " + C_operator->getUser() + " " + name + " :" + topic + "\n";
		C_operator->getSocketPtr()->bufferize(msg);
	}
	msg = ":" + server_name + " 353 " + C_operator->getUser() + " = " + this->name + " :" + this->user_list() + "\n";
	C_operator->getSocketPtr()->bufferize(msg);
	msg = ":" + server_name + " 366 " + C_operator->getUser() + " " + this->name + " :End of NAMES list.\n";
	C_operator->getSocketPtr()->bufferize(msg);
}

void				Channel::addUser(User *user)
{
	this->active_users.push_back(user);
	this->list_all_users.push_back(user->getNickname());

	std::string temp = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1";
	std::string msg = temp + " JOIN :"+ this->name + "\n";
	user->getSocketPtr()->bufferize(msg);
	if (this->topic != "")
	{
		msg = ":" + server_name + " 332 " + user->getUser() + " " + this->name + " :" + this->topic + "\n";
		user->getSocketPtr()->bufferize(msg);
	}
	msg = ":" + server_name + " 353 " + user->getUser() + " = " + this->name + " :" + this->user_list() + "\n";
	user->getSocketPtr()->bufferize(msg);
	msg = ":" + server_name + " 366 " + user->getUser() + " " + this->name + " :End of NAMES list.\n";
	user->getSocketPtr()->bufferize(msg);
}

std::string			Channel::user_list()
{
	std::string res;

	for (unsigned int i = 0; i < list_all_users.size(); ++i)
	{
		if(i == 0)
			res += list_all_users[i];
		else
			res += ", " + list_all_users[i];
	}
	return res;
}

std::string			Channel::getName()
{
	return this->name;
}

std::string			Channel::getTopic() const
{
	return this->topic;
}

std::string			&Channel::getServerName()
{
	return this->server_name;
}

void				Channel::setTopic(std::string const &topic) {
	this->topic = topic;
}

void				Channel::part(Socket *user)
{
	std::string msg;
	User *temp;

	for(unsigned int i = 0; i < active_users.size(); ++i)
	{
		if(active_users[i]->getSocketPtr() == user)
		{
			temp = active_users[i];
			msg = ":" + temp->getUser() + "!~" + temp->getUser() + "@127.0.0.1 PART " + this->name + "\n";
			temp->getSocketPtr()->bufferize(msg);
			active_users.erase(active_users.begin() + i);
			list_all_users.erase(list_all_users.begin() + i);
		}
	}
}

int				Channel::NumberOfUsers()
{
	return active_users.size();
}

Channel::~Channel()
{

}
