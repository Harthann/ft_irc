#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include "User.hpp"
# include <stdlib.h>
# include "mode_bool.hpp"
# include "utils.hpp"
# include "Socket.hpp"


class User;

class Channel
{
	public:
		Channel();
		Channel(std::string Name, User *channel_operator, std::string server_name);
		std::string			getName();
/*		bool				IsPrivate();
		bool				IsSecret();
		bool				IsInviteOnly();
		bool				IsModerate();
		bool				IsAnonymous();
		bool				KeyIsSet();
		bool				LimitUserSet();
		bool				NoMessageOutside();
		bool				TopicIsSettable();*/
		bool				check_if_flag_n_active(int n);
/*		void				setPrivate(int n, User *user);
		void				setSecret(int n, User *user);
		void				setInviteOnly(int n, User *user);
		void				setModerate(int n, User *user);
		void				setAnonymous(int n, User *user);
		void				setNoMessageOutside(int n, User *user);
		void				setTopicFlag(int n, User *user);*/
		void				setKey(int n, Commands &cmd, User *user);
		void				setLimitUser(int n, Commands &cmd, User *user);
		void				setModes(int n, User *user);
		void				addUser(User *user);
		void				part(Socket *user);
		void				Kick(User *ChannelOP, User *member, std::string msg = "No reason");
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
		std::string 		&getKey();
		unsigned long		getLimit();
		std::string			user_list();
		void				AddToInvitedUser(User * Member, User * Guest);
		void				delete_from_invited(User *user);
		void				AddVoiceUsers(User *Ch_operator, User *user);
		void				DelVoiceUsers(User *Ch_operator, User *user);
		bool				LimitCheck(User *user);
	protected:
		std::string				name;
		unsigned long			limit;
/*		bool					PrivateFlag;
		bool					SecretFlag;
		bool					InviteFlag;
		bool					ModerateFlag;
		bool					AnonymousFlag;
		bool					MessageOutsideFlag;
		bool					TopicSettableFlag;*/
		bool					KeyFlag;
		bool					LimitFlag;
		short					mode;
		std::string				key;
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
