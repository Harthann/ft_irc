#include "mode.hpp"

void	mode_channel(Commands &cmd, Socket *client, Server &server) {
	User	*temp_user = check_user(server.getClients(), client);
	Channel	*temp_channel = channel_exist(cmd[1], server);

	if(!temp_channel->CheckIfChannelOperator(temp_user) && cmd.length() >= 3)
	{
		std::string msg = ":" + server.getServerName() + REPLY(ERR_CHANOPRIVSNEEDED) + temp_channel->getName() + ":Your are not Channel Operator\r\n";
		temp_user->getSocketPtr()->bufferize(msg);
		return;
	}
	if (cmd.length() >= 3)
			mode_add_or_remove(cmd, temp_channel, temp_user);
	else
	{
		std::string StrMode = temp_channel->getStrMode();
		std::string msg = ":" + server.getServerName() + REPLY(RPL_CHANNELMODEIS) + temp_user->getNickname() + " " + temp_channel->getName() + " " + StrMode + "\r\n";
		temp_user->getSocketPtr()->bufferize(msg);
	}
}
