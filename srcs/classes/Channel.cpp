#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string Name, User *C_operator, std::string Server_name) : name(Name),
topic("")
{
	this->server_name = Server_name;
	this->channel_type = Name[0];
	this->active_users.push_back(C_operator);
	if (channel_type != '+')
		this->channel_operators.push_back(C_operator);
	else
		this->list_all_users.push_back(C_operator->getNickname());
	for(int i = 0; i < 16; ++i)
		clearing_a_bit(this->mode, i);
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
		else if(checking_a_bit(this->mode, MODERATE_FLAG) && this->CheckIfVoiceUser(this->active_users[i]))
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
	for(unsigned int i = 0; i < this->voice_users.size(); ++i)
	{
		if(this->voice_users[i]->getSocketPtr() == user)
			voice_users.erase(voice_users.begin() + i);
	}
	for(unsigned int i = 0; i < this->channel_operators.size(); ++i)
	{
		if(this->channel_operators[i]->getSocketPtr() == user)
			channel_operators.erase(channel_operators.begin() + i);
	}
}

void				Channel::Kick(User *ChannelOP, User *member, std::string msg)
{
	std::string msgg;

	msgg = ":" + ChannelOP->getUser() + "!~" + ChannelOP->getUser() + "@127.0.0.1 KICK " + this->name + " " + member->getNickname() + "  "  + msg +"\n";
	this->SendMsgToAll(msgg);
	for(unsigned int i = 0; i < this->active_users.size(); ++i)
	{
		if(this->active_users[i]->getNickname() == member->getNickname())
			active_users.erase(active_users.begin() + i);
	}
	for(unsigned int i = 0; i < this->voice_users.size(); ++i)
	{
		if(this->voice_users[i]->getNickname() == member->getNickname())
			voice_users.erase(voice_users.begin() + i);
	}
	for(unsigned int i = 0; i < this->channel_operators.size(); ++i)
	{
		if(this->channel_operators[i]->getNickname() == member->getNickname())
			channel_operators.erase(channel_operators.begin() + i);
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

void		Channel::setKey(int n, Commands &cmd, User *user)
{
	std::string msg;
	if(cmd.length() == 4 || (cmd.length() == 3 && cmd[2][0] == '-'))
	{
		if(CheckIfChannelOperator(user))
		{
			if(n)
			{
				msg = mode_msg(n, KEY_FLAG, user, this->name, cmd[3]);
				setting_a_bit(this->mode, KEY_FLAG);
				this->key = cmd[3];
			}
			else
			{
				msg = mode_msg(n, KEY_FLAG, user, this->name);
				clearing_a_bit(this->mode, KEY_FLAG);
			}
			this->SendMsgToAll(msg);
		}
	}
}

void		Channel::setLimitUser(int n, Commands &cmd, User *user)
{
	std::string msg;
	if(cmd.length() == 4 || (cmd.length() == 3 && cmd[2][0] == '-'))
	{
		if(CheckIfChannelOperator(user))
		{
			if(n)
			{
				msg = mode_msg(n, LIMIT_FLAG, user, this->name, cmd[3]);
				this->limit = atoi(cmd[3].c_str());
				setting_a_bit(this->mode, LIMIT_FLAG);
			}
			else
			{
				msg = mode_msg(n, LIMIT_FLAG, user, this->name);
				clearing_a_bit(this->mode, LIMIT_FLAG);
			}
			this->SendMsgToAll(msg);
		}
	}
}

void	Channel::setModes(int n, User *user, int n_mode)
{
	std::string msg;
	if(CheckIfChannelOperator(user))
	{
		msg = mode_msg(n, n_mode, user, this->name);
		if(n)
			setting_a_bit(this->mode, n_mode);
		else
			clearing_a_bit(this->mode, n_mode);
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

bool			Channel::LimitCheck(User *user)
{
	if(this->limit > active_users.size())
		return false;
	std::string msg = ":" + this->getServerName() + REPLY(ERR_CHANNELISFULL) + user->getUser() + " " + this->getName() + " :Cannot join channel (+l) \n";
	user->getSocketPtr()->bufferize(msg);
	return true;
}

std::string		& Channel::getKey()
{
	return this->key;
}

unsigned long	Channel::getLimit()
{
	return this->limit;
}

bool			Channel::check_if_flag_n_active(int n)
{
	return checking_a_bit(this->mode, n);
}

std::string						Channel::mode_msg(int n, int i, User *user, std::string channel_name, std::string keyLimit)
{
	std::string modes("ovaimnpstkl");
	std::string remove_add;
	std::string msg;

	msg = ":" + user->getUser() + "!~" + user->getUser() + "@127.0.0.1";
	remove_add = (n == 1) ? " +" : " -";
	remove_add += modes[i];
	if(keyLimit == "")
		msg += " MODE " + channel_name + remove_add + "\r\n";
	else
		msg += " MODE " + channel_name + remove_add + " " + keyLimit +"\r\n";

	return msg;
}

short							& Channel::getMode()
{
	return this->mode;
}

Channel::~Channel()
{

}
