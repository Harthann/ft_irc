#include "Socket.hpp"
#include "User.hpp"
#include <vector>
#include <iostream>

void	add_user(Socket *client, std::vector<User> &temp_user, std::string datas)
{
	unsigned long i;
	int n = 0;
	for (i = 0; i < temp_user.size(); i++)
	{
		if(temp_user[i].getSocket() == client)
		{
			n = 1;
			break;
		}
	}
	if(n == 1 && !temp_user[i].getStatus())
		temp_user[i].setDatas(datas);
	else if(n == 0)
		temp_user.push_back(User(client, datas));
}
