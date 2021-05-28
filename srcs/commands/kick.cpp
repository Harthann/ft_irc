#include "commands_prototypes.hpp"
#include "numeric_replies.hpp"

void	kick_command(Commands &cmd, Socket *client, Server &server) {
	std::string	response;
	User		*current_user;
	User		*targeted_user;
	Channel		*channel;

	current_user = check_user(server.getClients(), client);
	if (!(channel = channel_exist(cmd[1], server)))
		response.append(cmd[1] + ":No such channel");
	if (cmd.length() < 3 || cmd.length() > 4)
		response.append(":" + server.getServerName() + REPLY(ERR_NEEDMOREPARAMS) + current_user->getNickname() + " KICK :Syntax error");
	else
	{
		targeted_user = server.getUserByName(cmd[2]);
		if (!targeted_user)
			response.append(":" + server.getServerName() + REPLY(ERR_NOSUCHNICK) + cmd[2] + " :No suck nick or channel name");
		else if (current_user->flagIsSet(OPERATOR_FLAG))
		{
			if (channel_exist(cmd[1], server))
				targeted_user->partChannel(cmd[1]);
			else
				response.append(":" + server.getServerName() + REPLY(ERR_NOSUCHNICK) + cmd[1] + " :No suck nick or channel name");
			response.append(current_user->getID() + " KICK " + cmd[1] + " " + current_user->getNickname() + " :");
			if (cmd.length() == 4)
				response.append(cmd[3]);
			else
				response.append(current_user->getNickname());
			targeted_user->getSocketPtr()->bufferize(response);
		}
		else
		{
			response.append(std::string(":") + server.getServerName() + REPLY(ERR_CHANOPRIVSNEEDED) + current_user->getNickname() + " " + channel->getName() + " :You are not channel operator");
		}
	}
	
	client->bufferize(response);
}
