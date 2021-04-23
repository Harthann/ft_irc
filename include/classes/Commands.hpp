#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>
#include <exception>
#include "Socket.hpp"
#include "tokens.hpp"

class Commands
{
	
	public:
		Commands(std::string &datas);
		~Commands();

		void		add(std::string);
		std::string as_string();
		std::string name();
		std::string from();
		std::string &getCmdParam(size_t);
		size_t		length() const;
		bool		isValid();

		std::string &operator[](size_t);


		struct out_of_range : public std::exception {
			const char *what() const throw() {
				return ("access out of range parameter on command line");
			};
		};
	private:
		Commands(Commands const &);
		Commands &operator=(Commands const&);

		std::vector<std::string>	cmd;
		bool						valid;
};

#endif
