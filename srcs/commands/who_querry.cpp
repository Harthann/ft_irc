#include "commands_prototypes.hpp"

bool	wildcardMatch(std::string str, std::string pattern) {
	bool	bool_array[str.size()+1] [pattern.size()+1];

	for (std::string::size_type i = 0; i <= str.size(); ++i)
	{
		for (std::string::size_type j = 0; j <= pattern.size(); ++j)
		{
			bool_array[i][j] = 0;
		}
	}
	bool_array[0][0] = true;
	for (std::string::size_type i = 1; i <= str.size(); i++)
	{
		for (std::string::size_type j = 1; j <= pattern.size(); j++)
		{
			if (str[i-1] == pattern[j-1] || pattern[j-1] == '?')
				bool_array[i][j] = bool_array[i-1][j-1];

			else if (pattern[j-1] == '*')
				bool_array[i][j] = bool_array[i][j-1] || bool_array[i-1][j];
		}
	}

	return bool_array[str.size()][pattern.size()];
}

void	output_user(Socket *client, Server &server, User *current_user, User *targeted_user) {
	std::string	response;

	response.append(":" + server.getServerName() + REPLY(RPL_WHOREPLY) + current_user->getNickname() + " * " + targeted_user->getNickname() + " " + client->getHostName() + " " + server.getServerName() + " " + targeted_user->getNickname() + " H " + ":" + "0 " + targeted_user->getRealName());
	client->bufferize(response);
	response.clear();
}

void	list_all_users(Socket *client, Server &server) {
	User		*current_user;

	current_user = check_user(server.getClients(), client);
	for (Server::user_it it = server.getClients().begin(); it != server.getClients().end(); ++it) {
		if (!(*it)->flagIsSet(INVISIBLE_FLAG) || current_user->haveCommonChanel(*it))
			output_user(client, server, current_user, *it);
	}
}

void	who_querry(Commands &cmd, Socket *client, Server &server) {
	User	*current_user;

	current_user = check_user(server.getClients(), client);
	if (cmd.length() == 1)
		list_all_users(client, server);
	client->bufferize(":" + server.getServerName() + REPLY(RPL_ENDOFWHO) + current_user->getNickname() + " * :End of WHO list");
}
