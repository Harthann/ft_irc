#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string Name, User *C_operator, std::string server_name) : name(Name),
topic(""),
server_name(server_name),
PrivateFlag(false),
SecretFlag(false),
InviteFlag(false),
ModerateFlag(false),
channel_type(Name[0])
{

	this->active_users.push_back(C_operator);
//	this->channel_type = Name[0];
	if (channel_type != '+')
		this->channel_operators.push_back(C_operator);
	else
		this->list_all_users.push_back(C_operator->getNickname());
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
	std::cout << "msg = " << msg << std::endl;
}

void				Channel::addUser(User *user)
{
	this->active_users.push_back(user);

	std::string temp = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1";
	std::string msg = temp + " JOIN :"+ this->name + "\n";
	this->SendMsgToAll(msg);
	if (CheckIfInvited(user))
		delete_from_invited(user);
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
	std::string temp;

	for (unsigned int i = 0; i < this->active_users.size(); ++i)
	{
		if(this->CheckIfChannelOperator(this->active_users[i]))
			temp = "@" + this->active_users[i]->getNickname();
		else if(this->IsModerate() && this->CheckIfVoiceUser(this->active_users[i]))
			temp = "+" + this->active_users[i]->getNickname();
		else
			temp = this->active_users[i]->getNickname();
		if(i == 0)
			res += temp;
		else
			res += ", " + temp;
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

	for(unsigned int i = 0; i < this->active_users.size(); ++i)
	{
		if(this->active_users[i]->getSocketPtr() == user)
		{
			temp = active_users[i];
			msg = ":" + temp->getUser() + "!~" + temp->getUser() + "@127.0.0.1 PART " + this->name + "\n";
			this->SendMsgToAll(msg);
			active_users.erase(active_users.begin() + i);
		}
	}
}

int				Channel::NumberOfUsers()
{
	return active_users.size();
}

User			*Channel::getUserByName(std::string name)
{
	for(size_t i = 0; i < this->active_users.size(); ++i)
	{
		if(this->active_users[i]->getNickname() == name)
			return this->active_users[i];
	}
	return NULL;
}

void			Channel::Privilege(int n, User *user, Commands &cmd)
{
	User *Receiver = NULL;
	std::string	msg;

	if (cmd.length() == 4 && CheckIfChannelOperator(user))
	{
		Receiver = this->getUserByName(cmd[3]);
		if (Receiver != NULL)
		{
			std::string temp = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1";
			if(n == 0)
			{
				msg = temp + " MODE "+ this->name + " -o " + Receiver->getNickname() + "\n";
				for (size_t i = 0; i < this->channel_operators.size(); ++i)
				{
					if (Receiver->getNickname() == this->channel_operators[i]->getNickname())
						channel_operators.erase(channel_operators.begin() + i);
				}
			}
			else
			{
				msg = temp + " MODE "+ this->name + " +o " + Receiver->getNickname() + "\n";
				this->channel_operators.push_back(Receiver);
			}
			this->SendMsgToAll(msg);
		}
	}
}

bool			Channel::CheckIfChannelOperator(User *user)
{
	for (size_t i = 0; i < this->channel_operators.size(); ++i)
	{
		if(this->channel_operators[i]->getNickname() == user->getNickname())
			return true;
	}
	return false;
}

bool			Channel::CheckIfVoiceUser(User *user)
{
	for (size_t i = 0; i < this->voice_users.size(); ++i)
	{
		if(this->voice_users[i]->getNickname() == user->getNickname())
			return true;
	}
	return false;
}

void		Channel::SendMsgToAll(std::string msg, User *x)
{
	for (unsigned int i = 0; i < this->active_users.size(); ++i)
	{
		if (!x || x != this->active_users[i])
			this->active_users[i]->getSocketPtr()->bufferize(msg);
	}
}

bool		Channel::IsPrivate()
{
	return this->PrivateFlag;
}

bool		Channel::IsSecret()
{
	return this->SecretFlag;
}

bool		Channel::IsInviteOnly()
{
	return this->InviteFlag;
}

bool		Channel::IsModerate()
{
	return this->ModerateFlag;
}

void		Channel::setPrivate(int n, User *user)
{
	std::string msg;
	if(CheckIfChannelOperator(user))
	{
		std::string temp = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1";
		if(n)
		{
			msg = temp + " MODE "+ this->name + " +p \n";
			this->PrivateFlag = true;
		}
		else
		{
			msg = temp + " MODE "+ this->name + " -p \n";
			this->PrivateFlag = false;
		}
		this->SendMsgToAll(msg);
	}
}

void		Channel::setSecret(int n, User *user)
{
	std::string msg;
	if(CheckIfChannelOperator(user))
	{
		std::string temp = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1";
		if(n)
		{
			msg = temp + " MODE "+ this->name + " +s \n";
			this->SecretFlag = true;
		}
		else
		{
			msg = temp + " MODE "+ this->name + " -s \n";
			this->SecretFlag = false;
		}
		this->SendMsgToAll(msg);
	}
}

void		Channel::setInviteOnly(int n, User *user)
{
	std::string msg;
	if(CheckIfChannelOperator(user))
	{
		std::string temp = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1";
		if(n)
		{
			msg = temp + " MODE "+ this->name + " +i \n";
			this->InviteFlag = true;
		}
		else
		{
			msg = temp + " MODE "+ this->name + " -i \n";
			this->InviteFlag = false;
		}
		this->SendMsgToAll(msg);
	}
}

void		Channel::setModerate(int n, User *user)
{
	std::string msg;
	if(CheckIfChannelOperator(user))
	{
		std::string temp = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1";
		if(n)
		{
			msg = temp + " MODE "+ this->name + " +m \n";
			this->ModerateFlag = true;
		}
		else
		{
			msg = temp + " MODE "+ this->name + " -m \n";
			this->ModerateFlag = false;
		}
		this->SendMsgToAll(msg);
	}
}

void	Channel::AddToInvitedUser(User * Member, User *Guest)
{
	std::string temp;
	std::string msg;
	this->invited_users.push_back(Guest);

	temp = ":" + Member->getUser() + "!~" + Member->getUser() + "@127.0.0.1";
	msg = temp + " INVITE " + Guest->getNickname() + " " + this->name + "\n";
	Guest->getSocketPtr()->bufferize(msg);
}

bool	Channel::CheckIfInvited(User *user)
{
	for (size_t i = 0; i < this->invited_users.size(); ++i)
	{
		if(user == this->invited_users[i])
			return true;
	}
	return false;
}

void	Channel::delete_from_invited(User *user)
{
	for (size_t i = 0; i < this->invited_users.size(); ++i)
	{
		if(user == this->invited_users[i])
		{
			this->invited_users.erase(this->invited_users.begin() + i);
			break;
		}
	}
}

void	Channel::AddVoiceUsers(User *Ch_operator, User *user)
{
	std::string msg;
	if (!this->CheckIfChannelOperator(user))
	{
		this->voice_users.push_back(user);
		std::string temp = ":" + Ch_operator->getUser() + "!~" + Ch_operator->getUser() + "@127.0.0.1";
		msg = temp + " MODE " + this->name + " +v " + user->getNickname() +"\n";
		this->SendMsgToAll(msg);
	}
}

void	Channel::DelVoiceUsers(User *Ch_operator, User *user)
{
	std::string msg;
	if (!this->CheckIfChannelOperator(user))
	{
		std::string temp = ":" + Ch_operator->getUser() + "!~" + Ch_operator->getUser() + "@127.0.0.1";
		msg = temp + " MODE " + this->name + " -v " + user->getNickname() +"\n";
		this->SendMsgToAll(msg);
		for (size_t i = 0; i < this->voice_users.size(); ++i)
		{
			if(user == this->voice_users[i])
			{
				this->voice_users.erase(this->voice_users.begin() + i);
				break;
			}
		}
	}
}

Channel::~Channel()
{

}
