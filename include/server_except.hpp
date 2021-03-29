#ifndef SERVER_EXCEPT_HPP
# define SERVER_EXCEPT_HPP

#include <exception>
#include "ft_irc.hpp"
#include <sstream>

namespace se {

	struct	ServerException {

		ServerException(std::string err = "ServerException") : error(err) {};
		ServerException(struct sockaddr_in x, std::string err = "ServerException") : info(x), error(err)
		{
			error.append("\n");
			error.append(this->who());
		};

		const char *what() const throw() { return (error.c_str()); };
		const std::string who() const {
			std::string str;
			std::stringstream str_stream;
			
			str_stream << inet_ntoa(info.sin_addr) << ':' << ntohs(info.sin_port);
			str = str_stream.str();
			return (str);
		};

		struct sockaddr_in	info;
		std::string	error;
	};

	struct InvalidAccept : public se::ServerException {
		InvalidAccept() : ServerException("Couldn't accept connection") {};
		InvalidAccept(struct sockaddr_in info) : ServerException(info, "Couldn't accept connection") {};
	};

	struct InvalidBind : public se::ServerException {
		InvalidBind() : ServerException("Couldn't bind socket to port") {};
		InvalidBind(struct sockaddr_in info) : ServerException(info, "Couldn't bind socket to port") {};
	};

	struct SocketFailed : public se::ServerException {
		SocketFailed() : ServerException("Socket creation failed") {};
	};

	struct ConnectionFailed : public se::ServerException {
		ConnectionFailed() : ServerException("Failed to connect to distant host") {};
		ConnectionFailed(struct sockaddr_in info) : ServerException(info, "Failed to connect to distant host") {};
	};
	
	struct LostConnection : public se::ServerException {
		LostConnection() : ServerException("Lost connection to distant host") {};
		LostConnection(struct sockaddr_in info) : ServerException(info, "Lost connection to distant host") {};
	};

	struct SelectFailed : public se::ServerException {
		SelectFailed() : ServerException("Couldn't select request") {};
	};

}

#endif