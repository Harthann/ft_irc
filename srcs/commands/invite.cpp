#include "commands_prototypes.hpp"

void	InvitingUser(Commands &cmd, Socket *client, Server &server)
{
	User	*ChannelMember;
	User	*Guest;
	Channel	*channel;
	std::string msg;

	ChannelMember = check_user(server.getClients(), client);
	if (cmd.length() > 2)
	{
		channel = channel_exist(cmd[2], server);
		Guest =	server.getUserByName(cmd[1]);
		if (Guest != NULL && channel != NULL)
		{
			if (channel->getUserByName(ChannelMember->getNickname()) && !channel->getUserByName(Guest->getNickname()))
			{
				if ((channel->IsInviteOnly() && channel->CheckIfChannelOperator(ChannelMember))
					|| !channel->IsInviteOnly())
					channel->AddToInvitedUser(ChannelMember, Guest);
			}
			else if(!channel->getUserByName(ChannelMember->getNickname()))
			{
				msg = ":" + server.getServerName() + REPLY(ERR_NOTONCHANNEL) + channel->getName() + "\n";
				ChannelMember->getSocketPtr()->bufferize(msg);
			}
			else if(channel->getUserByName(Guest->getNickname()))
			{
				msg = ":" + server.getServerName() + REPLY(ERR_USERONCHANNEL) + Guest->getNickname() + " " + channel->getName() + " :is already on channel\n";
				ChannelMember->getSocketPtr()->bufferize(msg);
			}
		}
		else if (Guest == NULL)
		{
			msg = ":" + server.getServerName() + REPLY(ERR_NOSUCHNICK) + cmd[1] + " :No such nick/channel\r\n";
			ChannelMember->getSocketPtr()->bufferize(msg);
		}
	}
	else
	{
		msg = ":" + server.getServerName() + REPLY(ERR_NEEDMOREPARAMS) + cmd[0] + " :Not enough parameters\n";
		ChannelMember->getSocketPtr()->bufferize(msg);
	}
}
