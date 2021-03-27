#ifndef USER_HPP
#define USER_HPP

#include "Socket.hpp"
#include <string.h>
#include <vector>


class User
{
	public:
		User();
		User(Socket *client, std::string datas);
		~User();
		Socket *	getSocket() const;
		int			getStatus() const;
		void		displayinfo();
		void		setDatas(std::string datas);
	protected:
	private:
		std::string	nickname;
		void		setNICK(char *str);
		void		setPASS(char *str);
		void		setUSER(std::string datas);
		std::string	user;
		std::string	mode;
		int status;
		std::string	realname;
		std::string	password;
		Socket		*self;

};

#endif
