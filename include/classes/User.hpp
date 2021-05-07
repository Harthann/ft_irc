#ifndef USER_HPP
#define USER_HPP

#include "Socket.hpp"
#include "Commands.hpp"
#include "Channel.hpp"
#include <string.h>
#include <vector>

class Channel;

class User
{
	public:
		User();
		User(Socket *client, Commands cmd);
		~User();
		Socket *	getSocketPtr() const;
		int			getStatus() const;
		std::string	getNickname() const;
		std::string	getUser() const;

		void		displayinfo();
		void		setDatas(Commands cmd);
		void		ActiveChannel(Channel *ch);
		int			getSocket() const;
		void		partChannel(std::string ch);
		void		partChannels();

	protected:
	private:
		std::string	nickname;
		void		setNICK(std::string str);
		void		setPASS(std::string str);
		void		setUSER(Commands cmd);
		std::vector	<Channel *> current_channel;
		std::string	user;
		std::string	mode;
		int status;
		std::string	realname;
		std::string	password;
		Socket		*self;
};

#endif
