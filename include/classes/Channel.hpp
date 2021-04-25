#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include "User.hpp"

class Channel
{
	public:
		Channel();
		Channel(Channel const &);
		Channel &operator=(Channel const&);
		~Channel();
		typedef	std::vector<User>		user_vector;
	protected:
		std::string				name;
		user_vector				active_users;
		User					*channel_operator;
};

#endif
