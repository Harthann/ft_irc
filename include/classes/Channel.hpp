#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include "User.hpp"

class User;

class Channel
{
	public:
		Channel(std::string Name, User &channel_operator);
		std::string			getName();
		void				addUser(User &user);
		~Channel();
		typedef	std::vector<User>		user_vector;
	protected:
		Channel();
		std::string				name;
		user_vector				active_users;
		User					 * channel_operator;
};

#endif
