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
		std::string				topic;
		user_vector				active_users;
		User					 * channel_operator;
		std::string 			list_all_users();
		std::string				server_name;
};

#endif
