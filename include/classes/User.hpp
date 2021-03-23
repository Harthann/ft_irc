#ifndef USER_HPP
#define USER_HPP

#include <string>

/*
	a - user is flagged as away;
	i - marks a users as invisible;
	w - user receives wallops;
	r - restricted user connection;
	s - marks a user for receipt of server notices.
	o - operator flag;
	O - local operator flag;
*/

class User
{
	public:
		User();
		User(User const &);
		User &operator=(User const&);
		~User();
	protected:
		std::string nick;
		std::string	user;
		std::string	password;
};

#endif
