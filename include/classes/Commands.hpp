#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>
#include <exception>
#include "Socket.hpp"
#include "tokens.hpp"

#define CMD_TYPE 0
#define MSG_TYPE 1

class Commands
{
	
	public:
		Commands(std::string &datas, int = 0);
		Commands(Commands const &);
		Commands &operator=(Commands const&);
		~Commands();

		void		add(std::string);
		std::string as_string();
		std::string name();
		std::string from();
		void		setFrom(std::string);
		size_t 		length();
		std::string &getCmdParam(size_t);
		size_t		length() const;
		bool		isValid();
		void		setType(int &);
		const int	&getType() const;

		std::string &operator[](size_t);


		struct out_of_range : public std::exception {
			const char *what() const throw() {
				return ("access out of range parameter on command line");
			};
		};
	private:

		std::string					prefix;
		std::vector<std::string>	cmd;
		bool						valid;
		int							type;
};

#endif
