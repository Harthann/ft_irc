#ifndef USER_HPP
#define USER_HPP

#include "Socket.hpp"

class User
{
	public:
		User();
		~User();
	protected:
	private:
		std::string	nickname;
		std::string	user;
		int			mode;
		std::string	realname;
		std::string	password;
		Socket		*self;

};

#endif
