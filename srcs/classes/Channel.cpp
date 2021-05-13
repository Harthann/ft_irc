#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string Name, User &C_operator) : name(Name), topic("none")
{
	this->channel_operator = &C_operator;
	this->active_users.push_back(C_operator);
	std::string msg = ":" + C_operator.getUser()  + " JOIN Welcome to the newly created channel :" + Name + "\n";
	C_operator.getSocketPtr()->bufferize(msg);
	msg = ": 332 RPL_TOPIC " + name + " :" + topic + "\n";
	C_operator.getSocketPtr()->bufferize(msg);
	C_operator.getSocketPtr()->bufferize(this->list_all_users());

	// send(C_operator.getSocket(), msg.c_str(), msg.length(), 0);
}

void				Channel::addUser(User &user)
{
	this->active_users.push_back(user);
	std::string msg = ":" + user.getUser()  + " JOIN Welcome to the newly created channel :" + this->name + "\n";
	user.getSocketPtr()->bufferize(msg);
	msg = ": 332 RPL_TOPIC " + this->name + " :" + this->topic + "\n";
	user.getSocketPtr()->bufferize(msg);
	user.getSocketPtr()->bufferize(this->list_all_users());

	// send(user.getSocket(), msg.c_str(), msg.length(), 0);
}


std::string			Channel::getName()
{
	return this->name;
}

bool			Channel::checkPresence(User &x)
{
	for (user_vector::iterator it = active_users.begin(); it != active_users.end(); ++it)
	{
		if (x.getNickname() == (*it).getNickname())
			return true;
	}
	return false;
}

std::string		Channel::list_all_users()
{
	std::string user_list;

	user_list.append("List of all users : ");
	for (unsigned int i = 0; i < this->active_users.size(); ++i)
	{
		user_list.append(active_users[i].getUser());
		if(i + 1 < this->active_users.size())
			user_list.append(", ");
		else
			user_list.append("\n");
	}
	return user_list;
}

Channel::~Channel()
{

}
