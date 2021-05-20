#include "commands_prototypes.hpp"

void	InvitingUser(Commands &cmd, Socket *client, Server &server)
{
	User	*ChannelMember;
	User	*Guest;
	Channel	*channel;

	if (cmd.length() > 2)
	{
		channel = channel_exist(cmd[2], server);
		ChannelMember = check_user(server.getClients(), client);
		Guest =	server.getUserByName(cmd[1]);
		if (Guest != NULL && channel != NULL)
		{
			if (channel->getUserByName(ChannelMember->getNickname()))
			{
				if ((channel->IsInviteOnly() && channel->CheckIfChannelOperator(ChannelMember))
					|| !channel->IsInviteOnly())
					channel->AddToInvitedUser(ChannelMember, Guest);
			}
		}
	}
}
