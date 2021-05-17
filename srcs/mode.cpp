#include "mode.hpp"

void	mode_parser(Command &cmd, Socket *client, Server &server)
{
	User *temp_user = check_user(server.getClient());
	Channel *temp_channel =
}
