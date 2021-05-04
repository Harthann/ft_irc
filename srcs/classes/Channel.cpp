#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string Name, User &C_operator) : name(Name), topic(""), server_name("servernet")
{
	this->channel_operator = &C_operator;
	this->active_users.push_back(C_operator);
	std::string temp = ":" + C_operator.getUser() + "!~" + C_operator.getUser() + "@127.0.0.1";
	std::string msg = temp + " JOIN :"+ Name + "\n";
	C_operator.getSocketPtr()->bufferize(msg);
	if (this->topic != "")
	{
		msg = ":" + server_name + " 332 " + C_operator.getUser() + " " + name + " :" + topic + "\n";
		C_operator.getSocketPtr()->bufferize(msg);
	}
	msg = ":" + server_name + " 353 " + C_operator.getUser() + " = " + this->name + " :@" + C_operator.getNickname() + "\n";
	C_operator.getSocketPtr()->bufferize(msg);
	msg = ":" + server_name + " 366 " + C_operator.getUser() + " " + this->name + " :End of NAMES list.\n";
	C_operator.getSocketPtr()->bufferize(msg);
}

void				Channel::addUser(User &user)
{
	this->active_users.push_back(user);
	std::string temp = ":" + user.getUser() + "!~" + user.getUser() + "@127.0.0.1";
	std::string msg = temp + " JOIN :"+ this->name + "\n";
	user.getSocketPtr()->bufferize(msg);
	if (this->topic != "")
	{
		msg = ":" + server_name + " 332 " + user.getUser() + " " + this->name + " :" + this->topic + "\n";
		user.getSocketPtr()->bufferize(msg);
	}
	msg = ":" + server_name + " 353 " + user.getUser() + " = " + this->name + " :" + this->list_all_users();
	user.getSocketPtr()->bufferize(msg);
	msg = ":" + server_name + " 366 " + user.getUser() + " " + this->name + " :End of NAMES list.\n";
	user.getSocketPtr()->bufferize(msg);
}


std::string			Channel::getName()
{
	return this->name;
}

std::string		Channel::list_all_users()
{
	std::string user_list;

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
