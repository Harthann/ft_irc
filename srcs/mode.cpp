#include "mode.hpp"

void	VoiceUserCheck(Commands &cmd, Channel *channel, User *user, int n)
{
	User *VoiceUser;

	if(cmd.length() == 4 && checking_a_bit(channel->getMode(), MODERATE_FLAG))
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

int		mode_char_to_mode_bit(char c, Channel *channel)
{
	std::string mode("ovaimnpstkl");

	for(unsigned int i = 0; i < mode.length(); ++i)
	{
		if(c == mode[i])
		{
			if ((i == 7 && checking_a_bit(channel->getMode(), PRIVATE_FLAG)) || (i == 6 && checking_a_bit(channel->getMode(), SECRET_FLAG)))
				break ;
			return i;
		}
	}
	return -1;
}

void	mode_add_or_remove(Commands &cmd, Channel *channel, User *user)
{
	std::string mode = cmd[2];
	int	signe;
	int n_mode;

	signe = (mode[0] == '+') ? 1 : 0;
	for (size_t i = 1; i < mode.length(); ++i)
	{
		n_mode = mode_char_to_mode_bit(mode[i], channel);
		if (mode[i] == 'o')
			channel->Privilege(signe, user, cmd);
		else if(mode[i] == 'v')
			VoiceUserCheck(cmd, channel, user, signe);
		else if(mode[i] == 'k')
			channel->setKey(signe, cmd, user);
		else if(mode[i] == 'l')
			channel->setLimitUser(signe, cmd, user);
		else if(n_mode != -1)
			channel->setModes(signe, user, n_mode);
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
