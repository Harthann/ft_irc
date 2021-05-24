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
		bool				IsPrivate();
		bool				IsSecret();
		bool				IsInviteOnly();
		bool				IsModerate();
		bool				IsAnonymous();
		bool				NoMessageOutside();
		void				setPrivate(int n, User *user);
		void				setSecret(int n, User *user);
		void				setInviteOnly(int n, User *user);
		void				setModerate(int n, User *user);
		void				setAnonymous(int n, User *user);
		void				setNoMessageOutside(int n, User *user);
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
		bool				CheckIfChannelOperator(User *user);
		bool				CheckIfVoiceUser(User *user);
		bool				CheckIfInvited(User *user);
		User				*getUserByName(std::string name);
		std::string			user_list();
		void				AddToInvitedUser(User * Member, User * Guest);
		void				delete_from_invited(User *user);
		void				AddVoiceUsers(User *Ch_operator, User *user);
		void				DelVoiceUsers(User *Ch_operator, User *user);
	protected:
		std::string				name;
		bool					PrivateFlag;
		bool					SecretFlag;
		bool					InviteFlag;
		bool					ModerateFlag;
		bool					AnonymousFlag;
		bool					MessageOutsideFlag;
		user_vector				invited_users;
		std::string				topic;
		char					channel_type;
		user_vector				active_users;
		user_vector				voice_users;
		user_vector				channel_operators;
		string_vector 			list_all_users;
		std::string				server_name;
};

#endif
