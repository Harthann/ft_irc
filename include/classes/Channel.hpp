#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include "User.hpp"

class User;

class Channel
{
	public:
		Channel();
		Channel(std::string Name, User *channel_operator);
		std::string			getName();
		void				addUser(User *user);
		void				part(Socket *user);
		int					NumberOfUsers();
		~Channel();
		typedef	std::vector<User *>		user_vector;
		typedef	std::vector<std::string>		string_vector;

	protected:
		std::string				name;
		std::string				topic;
		char					channel_type;
		user_vector				active_users;
		User					*channel_operator;
		string_vector 			list_all_users;
		std::string				user_list();
		std::string				server_name;
};

#endif
