#include "mode.hpp"

void	VoiceUserCheck(Commands &cmd, Channel *channel, User *user, int n)
{
	User *VoiceUser;

	if(cmd.length() == 4 && channel->IsModerate())
	{
		VoiceUser = channel->getUserByName(cmd[3]);
		if (VoiceUser != NULL &&  channel->CheckIfChannelOperator(user))
		{
			if(n == 1)
				channel->AddVoiceUsers(user, VoiceUser);
			else
				channel->DelVoiceUsers(user, VoiceUser);
		}
	}
}

void	RemoveMode(Commands &cmd, Channel *channel, User *user)
{
	std::string mode = cmd[2];
	for (size_t i = 1; i < mode.length(); ++i)
	{
		if (mode[i] == 'o')
			channel->Privilege(0, user, cmd);
		else if(mode[i] == 's')
			channel->setSecret(0, user);
		else if(mode[i] == 'p')
			channel->setPrivate(0, user);
		else if(mode[i] == 'i')
			channel->setInviteOnly(0, user);
		else if(mode[i] == 'm')
			channel->setModerate(0, user);
		else if(mode[i] == 'n')
			channel->setNoMessageOutside(0, user);
		else if(mode[i] == 'v')
			VoiceUserCheck(cmd, channel, user, 0);
		else if(mode[i] == 'a')
			channel->setAnonymous(0, user);
		else if(mode[i] == 't')
			channel->setTopicFlag(0, user);
		else if(mode[i] == 'k')
			channel->setKey(0, cmd, user);
		else if(mode[i] == 'l')
			channel->setLimitUser(0, cmd, user);

	}
}

void	AddMode(Commands &cmd, Channel *channel, User *user)
{
	std::string mode = cmd[2];
	for (size_t i = 1; i < mode.length(); ++i)
	{
		if (mode[i] == 'o')
			channel->Privilege(1, user, cmd);
		else if(mode[i] == 's' && !channel->IsPrivate())
			channel->setSecret(1, user);
		else if(mode[i] == 'p' && !channel->IsSecret())
			channel->setPrivate(1, user);
		else if(mode[i] == 'i')
			channel->setInviteOnly(1, user);
		else if(mode[i] == 'm')
			channel->setModerate(1, user);
		else if(mode[i] == 'n')
			channel->setNoMessageOutside(1, user);
		else if(mode[i] == 'v')
			VoiceUserCheck(cmd, channel, user, 1);
		else if(mode[i] == 'a')
			channel->setAnonymous(1, user);
		else if(mode[i] == 't')
			channel->setTopicFlag(1, user);
		else if(mode[i] == 'k')
			channel->setKey(1, cmd, user);
		else if(mode[i] == 'l')
			channel->setLimitUser(1, cmd, user);

	}
}

void	mode_parser(Commands &cmd, Socket *client, Server &server)
{
	User *temp_user = check_user(server.getClients(), client);
	Channel *temp_channel = channel_exist(cmd[1], server);

	if (cmd.length() >= 3)
	{
		if (cmd[2].at(0) != '+')
			RemoveMode(cmd, temp_channel, temp_user);
		else
			AddMode(cmd, temp_channel, temp_user);
	}
}
