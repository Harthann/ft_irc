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
				if ((checking_a_bit(channel->getMode(), INVITE_FLAG) && channel->CheckIfChannelOperator(ChannelMember))
					|| !checking_a_bit(channel->getMode(), INVITE_FLAG))
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

void	KickUser(Commands &cmd, Socket *client, Server &server)
{
	User *ChannelOP;
	User *member;
	Channel *channel;
	std::string msg;

	ChannelOP = check_user(server.getClients(), client);
	if(cmd.length() > 2)
	{
		channel = channel_exist(cmd[1], server);
		member = server.getUserByName(cmd[2]);
		if(member != NULL && channel != NULL)
		{
			if(channel->CheckIfChannelOperator(ChannelOP) && channel->getUserByName(member->getNickname()))
			{
				if(cmd.length() == 3)
					channel->Kick(ChannelOP, member);
				else
					channel->Kick(ChannelOP, member, cmd[3]);
			}
			else if(!channel->getUserByName(ChannelOP->getNickname()))
			{
				msg = ":" + server.getServerName() + REPLY(ERR_NOTONCHANNEL) + channel->getName() + "\n";
				ChannelOP->getSocketPtr()->bufferize(msg);
			}
		}
		else if (member == NULL)
		{
			msg = ":" + server.getServerName() + REPLY(ERR_NOSUCHNICK) + cmd[2] + " :No such nick/channel\r\n";
			ChannelOP->getSocketPtr()->bufferize(msg);
		}
	}
	else
	{
		msg = ":" + server.getServerName() + REPLY(ERR_NEEDMOREPARAMS) + cmd[0] + " :Not enough parameters\n";
		ChannelOP->getSocketPtr()->bufferize(msg);
	}

}
