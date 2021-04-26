#ifndef USER_HPP
#define USER_HPP

#include "Socket.hpp"
#include "Commands.hpp"
#include <string.h>
#include <vector>

class User
{
	public:
		User();
		User(Socket *client, Commands cmd);
		~User();
		Socket *	getSocketPtr() const;
		int			getStatus() const;
		void		displayinfo();
		void		setDatas(Commands cmd);

		const std::string &getNick() const;
		int			getSocket() const;


	protected:
	private:
		std::string	nickname;
		void		setNICK(std::string str);
		void		setPASS(std::string str);
		void		setUSER(Commands cmd);
		std::string	user;
		std::string	mode;
		int status;
		std::string	realname;
		std::string	password;
		Socket		*self;
};

#endif
