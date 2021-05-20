#include "commands_prototypes.hpp"

void	InvitingUser(Commands &cmd, Socket *client, Server &server)
{
	User	*ChannelMember;
	User	*Guest;
	Channel	*Channel;

	ChannelMember = check_user(server.getClient(), client);
	Guest =
	Channel = channel_exist(cmd[2], server);
}
