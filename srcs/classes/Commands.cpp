#include "Commands.hpp"


Commands::Commands()
{
	for (int i = 0; i < 59; ++i)
	{
		list[tokens[i]] = &not_implemented;
	}
	list["PING"]  = &pong_response;
	list["DIE"]  = &exit_server;
	list["NOTICE"]  = &notice_command;
}

Commands::Commands(Commands const &)
{

}

Commands	&Commands::operator=(Commands const &)
{
	return (*this);
}

Commands::~Commands()
{

}


Commands::fcntl Commands::operator[](std::string &key)
{
	if (list.find(key) == list.end())
		return (list["WRONGCMD"]);
	return (list[key]);
}
