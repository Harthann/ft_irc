#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include "User.hpp"

class User;

class Channel
{
	public:
		Channel();
		Channel(std::string Name, User *channel_operator, std::string server_name);
		std::string			getName();
		void				addUser(User *user);
		void				part(Socket *user);
		int					NumberOfUsers();
		std::string			getTopic() const;
		std::string			&getServerName();
		void				setTopic(std::string const &topic);
		void				Privilege(int n, User *user, Commands &cmd);
		void				SendMsgToAll(std::string msg, User * = NULL);
		~Channel();
		typedef	std::vector<User *>		user_vector;
		typedef	std::vector<std::string>		string_vector;

	protected:
		std::string				name;
		std::string				topic;
		char					channel_type;
		user_vector				active_users;
		user_vector				channel_operators;
		string_vector 			list_all_users;
		std::string				user_list();
		User					*getUserByName(std::string name);
		bool					CheckIfChannelOperator(User *user);
		std::string				server_name;
};

#endif
