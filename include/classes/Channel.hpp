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
		~Channel();
		typedef	std::vector<User *>		user_vector;
		typedef	std::vector<std::string>		string_vector;
		bool				CheckIfChannelOperator(User *user);
		User				*getUserByName(std::string name);
		std::string				user_list();

	protected:
		std::string				name;
		std::string				topic;
		char					channel_type;
		user_vector				active_users;
		user_vector				channel_operators;
		string_vector 			list_all_users;
		std::string				server_name;
		void					SendMsgToAll(std::string msg);
};

#endif
