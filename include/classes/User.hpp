#ifndef USER_HPP
#define USER_HPP

#include "Socket.hpp"
#include <string.h>


class User
{
	public:
		User();
		User(Socket *client, std::string datas);
		~User();
		Socket *	getSocket() const;
		int			getStatus() const;
	protected:
	private:
		std::string	nickname;
		void		setNICK(char *str);
		void		setPASS(char *str);
		void		setUSER(char *str);
		std::string	user;
		int			mode;
		int status;
		std::string	realname;
		std::string	password;
		Socket		*self;

};

#endif
