#ifndef USER_HPP
#define USER_HPP

#include "Socket.hpp"
#include "Commands.hpp"
#include "Channel.hpp"
#include <string.h>
#include <vector>

class User
{
	public:
		User();
		User(Socket *client, Commands cmd);
		~User();
		Socket *	getSocket() const;
		int			getStatus() const;
		void		displayinfo();
		void		setDatas(Commands cmd);
	protected:
	private:
		std::string	nickname;
		void		setNICK(std::string str);
		void		setPASS(std::string str);
		void		setUSER(Commands cmd);
		Channel		*current_channel;
		std::string	user;
		std::string	mode;
		int status;
		std::string	realname;
		std::string	password;
		Socket		*self;
};

#endif
