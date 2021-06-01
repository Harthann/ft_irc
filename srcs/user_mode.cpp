
#include "mode.hpp"

char	get_flag(char c, User &targeted_user) {
	char	mode;

	mode = 0;
	switch (c) {
		case ('i') : {
			mode = INVISIBLE_FLAG;
			break ;
		}
		case ('s') : {
			mode = SERVER_NOTICES_FLAG;
			break ;
		}
		case ('w') : {
			mode = WALLOPS_FLAG;
			break ;
		}
		case ('o') : {
			mode = OPERATOR_FLAG;
			break ;
		}
	}
	return (mode);
}

void	mode_user(Commands &cmd, Socket *client, Server &server) {
	std::string	response;
	User		*current_user;
	User		*targeted_user;
	char		mode;

	current_user = check_user(server.getClients(), client);
	targeted_user = server.getUserByName(cmd[1]);
	if (!targeted_user)
	{
		response.append(":"
						+ server.getServerName()
						+ REPLY(ERR_NOSUCHNICK)
						+ cmd[1]
						+ " :No suck nick or channel name");
	}
	else
	{
		for (size_t i = 1; i < cmd[2].length(); ++i) {
			mode = get_flag(cmd[2][i], *targeted_user);
			if (mode)
			{
				if (cmd[2][0] == '+' || cmd[2][0] == '-')
				{
					if (cmd[2][0] == '+' && targeted_user->flagIsSet(mode))
						targeted_user->enableFlag(mode);
					else if (cmd[2][0] == '-' && !targeted_user->flagIsSet(mode))
						targeted_user->disableFlag(mode);
					response.append(current_user->getID()
									+ " MODE "
									+ targeted_user->getNickname()
									+ " :"
									+ cmd[2][0]
									+ cmd[2][i]);
				}
				else
					response.append(":"
									+ server.getServerName()
									+ REPLY(ERR_UMODEUNKNOWNFLAG)
									+ current_user->getNickname()
									+ " :Unknown mode");
				}
			else
				response.append(":"
								+ server.getServerName()
								+ REPLY(ERR_UMODEUNKNOWNFLAG)
								+ current_user->getNickname()
								+ " :Unknown mode \""
								+ cmd[2][0]
								+ cmd[2][i]
								+ "\"");
			client->bufferize(response);
			response.clear();
		}
	}
}
