#ifndef USER_HPP
#define USER_HPP

#include "Socket.hpp"
#include "Commands.hpp"
#include "Channel.hpp"
#include "numeric_replies.hpp"
#include <string.h>
#include <vector>

class Channel;

#define INVISIBLE_FLAG		0b10000000
#define SERVER_NOTICES_FLAG	0b01000000
#define WALLOPS_FLAG		0b00100000
#define OPERATOR_FLAG		0b00010000

class User
{
	public:
		User();
		User(Socket *client, Commands cmd, std::string server_name);
		~User();

		bool	operator==(const User &);

		Socket *				getSocketPtr() const;
		int						getStatus() const;
		std::string				getNickname() const;
		std::string				getUser() const;
		std::vector<Channel *>	&getChannels();

		void		displayinfo();
		void		setDatas(Commands cmd, std::string server_name);
		void		ActiveChannel(Channel *ch);
		int			getSocket() const;
		void		partChannel(std::string ch);
		void		partChannels();
		std::string	getID() const;

		/****************************************************************/
		/*							Flags								*/
		/****************************************************************/

		void		enableFlag(char mask);
		void		disableFlag(char mask);
		bool		flagIsSet(char mask);
		std::string	getIP() const;

		const std::string &getAwayMessage() const;
		void		setNICK(std::string str);

		void		setAwayMessage(std::string);
		bool		operator == (User *u2);

	protected:
	private:
		void		setPASS(std::string str);
		void		setUSER(Commands cmd, std::string server_name);

		std::string				nickname;
		std::vector<Channel *>	current_channel;
		std::string				user;
		std::string				mode;
		int 					status;
		std::string				realname;
		std::string				password;
		Socket					*self;
		char					flags;

		std::string				away_message;
};


#endif
