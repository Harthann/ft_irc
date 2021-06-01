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

int		mode_char_to_mode_bit(char c)
{
	std::string mode("ovaimnpstkl");

	for(int i = 0; i < mode.length; ++i)
	{
		if(c == mode[i])
			return i;
	}
	return -1;
}

void	RemoveMode(Commands &cmd, Channel *channel, User *user)
{
	std::string mode = cmd[2];
	int	signe;
	int n_mode;
	if(cmd[0] == '+')
		signe = 1;
	else 
		signe = 0;
	for (size_t i = 1; i < mode.length(); ++i)
	{
		n_mode = mode_char_to_mode_bit(mode[i]);
		if (mode[i] == 'o')
			channel->Privilege(0, user, cmd);
/*		else if(mode[i] == 's')
			channel->setSecret(0, user);
		else if(mode[i] == 'p')
			channel->setPrivate(0, user);
		else if(mode[i] == 'i')
			channel->setInviteOnly(0, user);
		else if(mode[i] == 'm')
			channel->setModerate(0, user);
		else if(mode[i] == 'n')
			channel->setNoMessageOutside(0, user);*/
		else if(mode[i] == 'v')
			VoiceUserCheck(cmd, channel, user, 0);
/*		else if(mode[i] == 'a')
			channel->setAnonymous(0, user);
		else if(mode[i] == 't')
			channel->setTopicFlag(0, user);*/
		else if(mode[i] == 'k')
			channel->setKey(0, cmd, user);
		else if(mode[i] == 'l')
			channel->setLimitUser(0, cmd, user);
		else if(n_mode != -1)
			channel->setModes(0, user, n_mode);
	}
}

void	AddMode(Commands &cmd, Channel *channel, User *user)
{
	std::string mode = cmd[2];
	for (size_t i = 1; i < mode.length(); ++i)
	{
		if (mode[i] == 'o')
			channel->Privilege(1, user, cmd);
/*		else if(mode[i] == 's' && !channel->IsPrivate())
			channel->setSecret(1, user);
		else if(mode[i] == 'p' && !channel->IsSecret())
			channel->setPrivate(1, user);
		else if(mode[i] == 'i')
			channel->setInviteOnly(1, user);
		else if(mode[i] == 'm')
			channel->setModerate(1, user);
		else if(mode[i] == 'n')
			channel->setNoMessageOutside(1, user);*/
		else if(mode[i] == 'v')
			VoiceUserCheck(cmd, channel, user, 1);
/*		else if(mode[i] == 'a')
			channel->setAnonymous(1, user);
		else if(mode[i] == 't')
			channel->setTopicFlag(1, user);*/
		else if(mode[i] == 'k')
			channel->setKey(1, cmd, user);
		else if(mode[i] == 'l')
			channel->setLimitUser(1, cmd, user);
		else if(n_mode != -1)
			channel->setModes(1, user, n_mode);

	}
}

void	mode_parser(Commands &cmd, Socket *client, Server &server)
{
	User *temp_user = check_user(server.getClients(), client);

	if (channel_exist(cmd[1], server))
		mode_channel(cmd, client, server);
	else if (server.IsUserOnServer(cmd[1]))
		mode_user(cmd, client, server);
	else
		client->bufferize(":" + server.getServerName() + REPLY(ERR_NOSUCHCHANNEL) + temp_user->getNickname() + " " + cmd[1] + " :No such channel");
}
